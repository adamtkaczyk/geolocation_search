#pragma once

#include <geolocation.hpp>
#include <geolocation_client.hpp>
#include <databse_client_interface.hpp>
#include <unordered_map>

class GeolocationSearch {
public:
    GeolocationSearch(std::unique_ptr<GeolocationClientInterface> geolocation_client, std::unique_ptr<DatabaseClientInterface> databse_client);
    std::optional<GeoLocation> getIPGeoLocation(const std::string& host);
    bool deleteLocation(const std::string& host);
private:
    const std::string parseInput(const std::string& host);

    std::unique_ptr<GeolocationClientInterface> geolocation_client_;
    std::unique_ptr<DatabaseClientInterface> databse_client_;
    std::unordered_map<std::string, GeoLocation> cache_;
};
