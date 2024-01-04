#pragma once

#include <string>

struct GeoLocationConfiguration {
    enum class DatabaseType {
        UNKONOW = 0,
        POSTGRESQL = 1
    };
    DatabaseType db_type;
    std::string db_name;
    std::string db_password;
    std::string db_user;
    std::string db_hostname;
    unsigned db_port;

    enum class GeoLocationClientType {
        UNKONOW = 0,
        IPSTACK = 1
    };

    GeoLocationClientType geolocation_client_type;
    std::string geolocation_client_url;
    std::string api_key;

    void readFromIniFile(const std::string& configuration_file);
};
