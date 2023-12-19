#pragma once

#include <geolocation.hpp>
#include <optional>

class DatabaseClientInterface {
public:
    virtual ~DatabaseClientInterface() = default;

    virtual bool store(const GeoLocation& geolocation) = 0;
    virtual std::optional<GeoLocation> getGeolocation(const std::string& host) = 0;
};
