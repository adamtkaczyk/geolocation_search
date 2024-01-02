#pragma once

#include <databse_client_interface.hpp>

class NullDatabse : public DatabaseClientInterface {
    ~NullDatabse() = default;

    virtual void connect() {};
    bool storeGeoLocation(const GeoLocation& geolocation) { return true; }
    bool deleteGeoLocation(const std::string& host) { return true; }
    virtual std::optional<GeoLocation> getGeolocation(const std::string& host) { return std::nullopt; }
};
