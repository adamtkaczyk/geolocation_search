#include <geolocation_search.hpp>

GeolocationSearch::GeolocationSearch(std::unique_ptr<GeolocationClientInterface> geolocation_client, std::unique_ptr<DatabaseClientInterface> databse_client)
    : geolocation_client_(std::move(geolocation_client))
    , databse_client_(std::move(databse_client))
{

}

std::optional<GeoLocation> GeolocationSearch::getIPGeoLocation(const std::string& host)
{
    const auto parsed_host = parseInput(host);
    if (cache_.find(parsed_host) == cache_.end()) {
        auto geolocation = databse_client_->getGeolocation(parsed_host);
        if (!geolocation) {
            std::cout << "Geolocation for host: " << parsed_host << " not present in database" << std::endl;
            geolocation = geolocation_client_->getIPGeoLocation(parsed_host);
        }

        if (!geolocation) {
            return std::nullopt;
        }
        cache_.insert({parsed_host, *geolocation});
        //TODO: consider what to do if store in db not sucess? Do it async to not blocking?
        databse_client_->store(*geolocation);
    }

    return cache_.at(parsed_host);
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
