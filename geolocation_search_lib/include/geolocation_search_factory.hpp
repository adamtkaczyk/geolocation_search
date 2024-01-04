#pragma once

#include <geolocation_search.hpp>
#include <geolocation_configuration.hpp>

class GeoLocationFactory {
public:
    static std::unique_ptr<GeolocationSearch> create(const GeoLocationConfiguration& configuration);
private:
    static std::unique_ptr<GeolocationClientInterface> createGeolocationClient(const GeoLocationConfiguration& configuration);
    static std::unique_ptr<DatabaseClientInterface> createDatabseClient(const GeoLocationConfiguration& configuration);
};
