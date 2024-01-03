#include <geolocation_search.hpp>

#include <spdlog/spdlog.h>

GeolocationSearch::GeolocationSearch(std::unique_ptr<GeolocationClientInterface> geolocation_client, std::unique_ptr<DatabaseClientInterface> databse_client)
    : geolocation_client_(std::move(geolocation_client))
    , databse_client_(std::move(databse_client))
{
}

std::optional<GeoLocation> GeolocationSearch::getIPGeoLocation(const std::string& host)
{
    const auto parsed_host = parseInput(host);
    spdlog::info("Get geolocation for host: {}", parsed_host);
    if (cache_.find(parsed_host) == cache_.end()) {
        spdlog::debug("Geolocation for host: {} not present in local cache", parsed_host);
        auto geolocation = databse_client_->getGeolocation(parsed_host);
        if (!geolocation) {
            spdlog::info("Geolocation for host: {} not present in database", parsed_host);
            geolocation = geolocation_client_->getIPGeoLocation(parsed_host);

            if (!geolocation) {
                return std::nullopt;
            }

            //TODO: consider what to do if store in db not sucess? Do it async to not blocking?
            //even if everythin will be fine maybe better not waiting on finished
            databse_client_->storeGeoLocation(*geolocation);
        }

        cache_.insert({parsed_host, *geolocation});
    }

    return cache_.at(parsed_host);
}

bool GeolocationSearch::deleteLocation(const std::string& host)
{
    const auto parsed_host = parseInput(host);
    spdlog::info("Delete geolocation for host: {}", parsed_host);
    if (cache_.find(parsed_host) != cache_.end()) {
        spdlog::debug("Delete host {} from local cache", parsed_host);
        cache_.erase(parsed_host);
    }

    return databse_client_->deleteGeoLocation(parsed_host);
}

const std::string GeolocationSearch::parseInput(const std::string& host)
{
    static std::vector<std::string> prefixes = {"http://", "https://"};
    for (const auto& prefix : prefixes) {
        if (prefix == host.substr(0, prefix.size())) {
            return host.substr(prefix.size());
        }
    }

    return host;
}
