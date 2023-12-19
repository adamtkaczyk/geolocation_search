#pragma once

#include <geolocation.hpp>
#include <string>
#include <optional>

class GeolocationClientInterface {
public:
    virtual ~GeolocationClientInterface() = default;
    virtual std::optional<GeoLocation> getIPGeoLocation(const std::string& host) = 0;
};
