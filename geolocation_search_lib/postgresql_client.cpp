#include <postgresql_client.hpp>

#include <spdlog/spdlog.h>

PostgreSQLClient::PostgreSQLClient(const std::string& database, const std::string& username, const std::string& password, const std::string& hostname, const unsigned port)
    : database_(database)
    , username_(username)
    , password_(password)
    , hostname_(hostname)
    , port_(port)
{
}

PostgreSQLClient::~PostgreSQLClient()
{
}

void PostgreSQLClient::connect()
{
    try {
        spdlog::info("Try connect to PostgreSQL database {} user = {}  hostaddr = {} port = {}", database_, username_, hostname_, port_);
        connection_ = std::make_unique<pqxx::connection>("dbname = " + database_ + " user = " + username_ + " password = " + password_ + " hostaddr = " + hostname_ + " port = " + std::to_string(port_));
    } catch (std::exception& e) {
        std::string message = "Can't connect to database: " + std::string{e.what()};
        spdlog::error(message);
        throw DatabaseException(message);
    }
}

bool PostgreSQLClient::storeGeoLocation(const GeoLocation& geolocation)
{
    spdlog::info("PostgreSQLClient store geolocation for host {} in database", geolocation.host);
    unsigned try_number = 0;
    bool success = false;

    do {
        try {
            try_number++;
            if (!connection_ || !connection_->is_open()) {
                spdlog::error("connection not exist or is close, try reconnect");
                connect();
                continue;
            }
            pqxx::transaction tx{*connection_};

            const std::string sql = "insert into geolocation_hots (host, ip, type, continent_code, continent_name, country_code, country_name, region_code, region_name, city, zip, latitude, longitude) values ("
                                    + tx.quote(geolocation.host) + ", "
                                    + tx.quote(geolocation.ip) + ", "
                                    + tx.quote(geolocation.type) + ", "
                                    + tx.quote(geolocation.continent_code) + ", "
                                    + tx.quote(geolocation.continent_name) + ", "
                                    + tx.quote(geolocation.country_code) + ", "
                                    + tx.quote(geolocation.country_name) + ", "
                                    + tx.quote(geolocation.region_code) + ", "
                                    + tx.quote(geolocation.region_name) + ", "
                                    + tx.quote(geolocation.city) + ", "
                                    + tx.quote(geolocation.zip) + ", "
                                    + std::to_string(geolocation.latitude) + ", "
                                    + std::to_string(geolocation.longitude) + ");";

            spdlog::debug("Execute query: {}", sql);
            tx.exec(sql.c_str());
            tx.commit();
            success = true;
        } catch (const pqxx::broken_connection& e) {
            spdlog::error("connection is broken, try reconnect: {}", e.what());
            connect();
        } catch (const std::exception &e) {
            spdlog::error("Can't store GeoLocation in database, exception: {}", e.what());
            break;
        }
    } while (!success && try_number < retry);

    return success;
}

bool PostgreSQLClient::deleteGeoLocation(const std::string& host)
{
    spdlog::info("PostgreSQLClient delete host {} from database", host);
    if(host.empty()) {
        return false;
    }
    unsigned try_number = 0;
    bool success = false;

    do {
        try {
            try_number++;
            if (!connection_ || !connection_->is_open()) {
                spdlog::error("connection not exist or is close, try reconnect");
                connect();
                continue;
            }
            pqxx::transaction tx{*connection_};

            const std::string sql = "delete from geolocation_hots where host=" + tx.quote(host) + ";";

            spdlog::debug("Execute query: {}", sql);
            tx.exec(sql.c_str());
            tx.commit();
            success = true;
        } catch (const pqxx::broken_connection& e) {
            spdlog::error("connection is broken, try reconnect: {}", e.what());
            connect();
        } catch (const std::exception &e) {
            spdlog::error("Can't delete GeoLocation from database, exception: {}", e.what());
            break;
        }
    } while (!success && try_number < retry);

    return success;
}

std::optional<GeoLocation> PostgreSQLClient::getGeolocation(const std::string& host)
{
    spdlog::info("PostgreSQLClient read geolocation for host {} from database", host);
    std::optional<GeoLocation> geolocation;
    if(host.empty()) {
        return geolocation;
    }

    unsigned try_number = 0;
    bool success = false;

    do {
        try {
            try_number++;
            if (!connection_ || !connection_->is_open()) {
                spdlog::error("connection not exist or is close, try reconnect");
                connect();
                continue;
            }
            pqxx::transaction tx{*connection_};

            const std::string sql = "select host, ip, type, continent_code, continent_name, country_code, country_name, region_code, region_name, city, zip, latitude, longitude from geolocation_hots where host=" + tx.quote(host) + ";";

            spdlog::debug("Execute query: {}", sql);
            auto result = tx.exec(sql.c_str());
            if (result.size() == 1) {
                geolocation = GeoLocation{};
                geolocation->host = result.at(0)["host"].as<std::string>();
                geolocation->ip = result.at(0)["ip"].as<std::string>();
                geolocation->type = result.at(0)["type"].as<std::string>();
                geolocation->continent_code = result.at(0)["continent_code"].as<std::string>();
                geolocation->continent_name = result.at(0)["continent_name"].as<std::string>();
                geolocation->country_code = result.at(0)["country_code"].as<std::string>();
                geolocation->country_name = result.at(0)["country_name"].as<std::string>();
                geolocation->region_code = result.at(0)["region_code"].as<std::string>();
                geolocation->region_name = result.at(0)["region_name"].as<std::string>();
                geolocation->city = result.at(0)["city"].as<std::string>();
                geolocation->zip = result.at(0)["zip"].as<std::string>();
                geolocation->latitude = result.at(0)["latitude"].as<double>();
                geolocation->longitude = result.at(0)["longitude"].as<double>();

                success = true;
            } else if (result.size() > 1) {
                spdlog::warn("Database return more then one result");
            }
            tx.commit();
        } catch (const pqxx::broken_connection& e) {
            spdlog::error("connection is broken, try reconnect: {}", e.what());
            connect();
        } catch (const std::exception &e) {
            spdlog::error("Can't read GeoLocation from database, exception: {}", e.what());
            break;
        }
    } while (!success && try_number < retry);

    return geolocation;
}
