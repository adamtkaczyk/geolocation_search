#pragma once

#include <databse_client_interface.hpp>

class NullDatabse : public DatabaseClientInterface {
    ~NullDatabse() = default;

    bool store(const GeoLocation& geolocation) { return true; }
    virtual std::optional<GeoLocation> getGeolocation(const std::string& host) { return std::nullopt; }
};
