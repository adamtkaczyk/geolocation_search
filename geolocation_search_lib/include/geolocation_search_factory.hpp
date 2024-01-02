#pragma once

#include <geolocation_search.hpp>

//TODO: move to other file, refactor some config paramiter specific only for one type
struct GeoLocationConfiguration {
    enum class DatabaseType {
        UNKONOW = 0,
        POSTGRESQL = 1
    };
    std::string db_name;
    std::string db_password;
    std::string db_user;
    std::string db_hostname;
    unsigned db_port;

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
private:
    static std::unique_ptr<GeolocationClientInterface> createGeolocationClient(const GeoLocationConfiguration& configuration);
    static std::unique_ptr<DatabaseClientInterface> createDatabseClient(const GeoLocationConfiguration& configuration);
};
