#include <geolocation_configuration.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <spdlog/spdlog.h>

void GeoLocationConfiguration::readFromIniFile(const std::string& configuration_file)
{
    spdlog::info("Read configuration from file: {}", configuration_file);
    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini(configuration_file, pt);

    auto db_type_str = pt.get<std::string>("database.type");
    if (db_type_str == "PostgreSQL") {
        db_type = GeoLocationConfiguration::DatabaseType::POSTGRESQL;
        db_name = pt.get<std::string>("database.name");
        db_password = pt.get<std::string>("database.password");
        db_user = pt.get<std::string>("database.user");
        db_hostname = pt.get<std::string>("database.hostname");
        db_port = pt.get<unsigned>("database.port");
    } else {
        throw std::runtime_error("Unsupport database type");
    }

    auto datasource_type_str = pt.get<std::string>("datasource.type");
    if (datasource_type_str == "ipstack") {
        geolocation_client_type = GeoLocationConfiguration::GeoLocationClientType::IPSTACK;
        geolocation_client_url = pt.get<std::string>("datasource.url");;
        api_key = pt.get<std::string>("datasource.api_key");;
    } else {
        throw std::runtime_error("Unsupport datasource type");
    }

}

