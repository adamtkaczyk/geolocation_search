#include "databse_client_interface.hpp"

class PostgreSQLClient : public DatabaseClientInterface {
public:
    PostgreSQLClient();
    bool store(const GeoLocation& geolocation) final;
    std::optional<GeoLocation> getGeolocation(const std::string& host) final;
};
