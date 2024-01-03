#include "databse_client_interface.hpp"

#include <pqxx/pqxx>

class PostgreSQLClient : public DatabaseClientInterface {
public:
    PostgreSQLClient(const std::string& database, const std::string& username, const std::string& password, const std::string& hostname, const unsigned port);
    ~PostgreSQLClient();

    void connect() override final;
    bool storeGeoLocation(const GeoLocation& geolocation) override final;
    bool deleteGeoLocation(const std::string& host) override final;
    std::optional<GeoLocation> getGeolocation(const std::string& host) override final;
private:
    const std::string database_;
    const std::string& username_;
    const std::string& password_;
    const std::string& hostname_;
    const unsigned port_;
    const short retry = 3;
    std::unique_ptr<pqxx::connection> connection_;
};
