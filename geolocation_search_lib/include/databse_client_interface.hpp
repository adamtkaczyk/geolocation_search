#pragma once

#include <geolocation.hpp>
#include <optional>
#include <stdexcept>

class DatabaseException : public std::runtime_error {
public:
    DatabaseException(const std::string& error_message)
        : std::runtime_error(error_message)
    {}
};

class DatabaseClientInterface {
public:
    virtual ~DatabaseClientInterface() = default;

    virtual void connect() = 0;
    virtual bool storeGeoLocation(const GeoLocation& geolocation) = 0;
    virtual bool deleteGeoLocation(const std::string& host) = 0;
    virtual std::optional<GeoLocation> getGeolocation(const std::string& host) = 0;
};
