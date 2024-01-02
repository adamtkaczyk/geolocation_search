#include <geolocation_search_factory.hpp>

#include <geolocation_client.hpp>
#include <postgresql_client.hpp>

std::unique_ptr<GeolocationSearch> GeoLocationFactory::create(const GeoLocationConfiguration& configuration)
{
    auto databse_client = createDatabseClient(configuration);
    databse_client->connect();

    auto geolocation_client = createGeolocationClient(configuration);

    return std::make_unique<GeolocationSearch>(std::move(geolocation_client), std::move(databse_client));
}

std::unique_ptr<GeolocationClientInterface> GeoLocationFactory::createGeolocationClient(const GeoLocationConfiguration& configuration)
{
    return std::make_unique<GeolocationClient>(configuration.geolocation_client_url, configuration.api_key);
}

std::unique_ptr<DatabaseClientInterface> GeoLocationFactory::createDatabseClient(const GeoLocationConfiguration& configuration)
{
    return std::make_unique<PostgreSQLClient>(configuration.db_name, configuration.db_user, configuration.db_password, configuration.db_hostname, configuration.db_port);
}
