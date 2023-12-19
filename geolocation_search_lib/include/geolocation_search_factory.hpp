#pragma once

#include <geolocation_search.hpp>

//TODO: move to other file, refactor some config paramiter specific only for one type
struct GeoLocationConfiguration {
    enum class DatabaseType {
        UNKONOW = 0,
        POSTGRESQL = 1
    };

    enum class GeoLocationClientType {
        UNKONOW = 0,
        IPSTACK = 1
    };

    DatabaseType database_type;
    GeoLocationClientType geolocation_client_type;
    std::string geolocation_client_url;
    std::string api_key;
};

class GeoLocationFactory {
public:
    static std::unique_ptr<GeolocationSearch> create(const GeoLocationConfiguration& configuration);
};
