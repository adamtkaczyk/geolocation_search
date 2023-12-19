#include <postgresql_client.hpp>

PostgreSQLClient::PostgreSQLClient()
{

}

bool PostgreSQLClient::store(const GeoLocation& geolocation)
{
    return true;
}

std::optional<GeoLocation> PostgreSQLClient::getGeolocation(const std::string& host)
{
    return std::nullopt;
}
