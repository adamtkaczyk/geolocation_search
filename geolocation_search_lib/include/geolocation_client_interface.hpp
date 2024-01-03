#pragma once

#include <geolocation.hpp>
#include <string>
#include <optional>
#include <stdexcept>

class GeolocationClientException : public std::runtime_error {
public:
    GeolocationClientException(const std::string& error_message)
        : std::runtime_error(error_message)
    {}
};

class GeolocationClientInterface {
public:
    virtual ~GeolocationClientInterface() = default;
    virtual std::optional<GeoLocation> getIPGeoLocation(const std::string& host) = 0;
};
