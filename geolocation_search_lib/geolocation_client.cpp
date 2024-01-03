#include <geolocation_client.hpp>


#include <cpprest/uri.h>
#include <string>
#include <iostream>

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;

GeolocationClient::GeolocationClient(const std::string host, const std::string api_key)
    : http_client_(host)
    , api_key_(api_key)
{
}

std::optional<GeoLocation> GeolocationClient::getIPGeoLocation(const std::string& host)
{    
    auto response = http_client_.request(methods::GET, uri_builder(U(host)).append_query(U("access_key"), api_key_).to_string()).get();

    if (response.status_code() != 200) {
        throw GeolocationClientException("Returned " + std::to_string(response.status_code()));
	}

    auto response_json = response.extract_json().get();

    auto geolocation = parseJson(response_json);
    if (geolocation) {
        geolocation->host = host;
    }
    return geolocation;
}

std::optional<GeoLocation> GeolocationClient::parseJson(json::value& json)
{
    GeoLocation geolocation;
    if (!json.has_double_field("latitude")) {
        throw GeolocationClientException("Field latitude not exist in json response");
    }

    if (!json.has_double_field("longitude")) {
        throw GeolocationClientException("Field longitude not exist in json response");
    }

    geolocation.latitude = json[U("latitude")].as_double();
    geolocation.longitude = json[U("longitude")].as_double();

    if (json.has_string_field("ip")) {
        geolocation.ip = json[U("ip")].as_string();
    }

    if (json.has_string_field("type")) {
        geolocation.type = json[U("type")].as_string();
    }

    if (json.has_string_field("continent_code")) {
        geolocation.continent_code = json[U("continent_code")].as_string();
    }

    if (json.has_string_field("continent_name")) {
        geolocation.continent_name = json[U("continent_name")].as_string();
    }

    if (json.has_string_field("country_code")) {
        geolocation.country_code = json[U("country_code")].as_string();
    }

    if (json.has_string_field("country_name")) {
        geolocation.country_name = json[U("country_name")].as_string();
    }

    if (json.has_string_field("region_code")) {
        geolocation.region_code = json[U("region_code")].as_string();
    }

    if (json.has_string_field("region_name")) {
        geolocation.region_name = json[U("region_name")].as_string();
    }

    if (json.has_string_field("city")) {
        geolocation.city = json[U("city")].as_string();
    }

    if (json.has_string_field("zip")) {
        geolocation.zip = json[U("zip")].as_string();
    }

    return geolocation;
}
