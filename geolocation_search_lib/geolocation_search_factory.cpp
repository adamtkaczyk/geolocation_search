#include <geolocation_search_factory.hpp>

#include <geolocation_client.hpp>
#include <postgresql_client.hpp>

std::unique_ptr<GeolocationSearch> GeoLocationFactory::create(const GeoLocationConfiguration& configuration)
{
    auto databse_client = std::make_unique<PostgreSQLClient>();
    auto geolocation_client = std::make_unique<GeolocationClient>(configuration.geolocation_client_url, configuration.api_key);

    return std::make_unique<GeolocationSearch>(std::move(geolocation_client), std::move(databse_client));
}
