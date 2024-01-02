#pragma once

#include <string>

struct GeoLocation {
    std::string host;
    std::string ip;
    std::string type;
    std::string continent_code;
    std::string continent_name;
    std::string country_code;
    std::string country_name;
    std::string region_code;
    std::string region_name;
    std::string city;
    std::string zip;
    double latitude;
    double longitude;
};
