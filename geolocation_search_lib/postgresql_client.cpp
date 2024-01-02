#include <postgresql_client.hpp>

#include <iostream>

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
    connection_ = std::make_unique<pqxx::connection>("dbname = " + database_ + " user = " + username_ + " password = " + password_ + " hostaddr = " + hostname_ + " port = " + std::to_string(port_));
}

bool PostgreSQLClient::storeGeoLocation(const GeoLocation& geolocation)
{   
    try {
        if (!connection_ || !connection_->is_open()) {
            std::cout << "connection not exist or is not open" << std::endl;
            return false;
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

        tx.exec(sql.c_str());
        tx.commit();
        return true;
    } catch (const std::exception &e) {
        std::cout << "Can't store GeoLocation in database, exception: " << e.what() << std::endl;
        return false;
    }
}

bool PostgreSQLClient::deleteGeoLocation(const std::string& host)
{
    std::cout << "PostgreSQLClient::deleteGeoLocation: " << host << std::endl;
    if(host.empty()) {
        return false;
    }
    try {
        if (!connection_ || !connection_->is_open()) {
            std::cout << "connection not exist or is not open" << std::endl;
            return false;
        }
        pqxx::transaction tx{*connection_};

        const std::string sql = "delete from geolocation_hots where host=" + tx.quote(host) + ";";
        std::cout << "Execute query: " << sql << std::endl;
        tx.exec(sql.c_str());
        tx.commit();
        return true;
    } catch (const std::exception &e) {
        std::cout << "Can't delete GeoLocation in database, exception: " << e.what() << std::endl;
        return false;
    }
}

std::optional<GeoLocation> PostgreSQLClient::getGeolocation(const std::string& host)
{
    std::optional<GeoLocation> geolocation;
    if(host.empty()) {
        return geolocation;
    }

    try {
        if (!connection_ || !connection_->is_open()) {
            std::cout << "connection not exist or is not open" << std::endl;
            return std::nullopt;
        }
        pqxx::transaction tx{*connection_};

        const std::string sql = "select host, ip, continent_code, continent_name, country_code, country_name, region_code, region_name, city, zip, latitude, longitude from geolocation_hots where host=" + tx.quote(host) + ";";
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
        } else if (result.size() > 1) {
            std::cout << "Database return more then one result" << std::endl;
        }
        tx.commit();
    } catch (const std::exception &e) {
        std::cout << "Can't delete GeoLocation in database, exception: " << e.what() << std::endl;
        return std::nullopt;
    }

    return geolocation;
}
