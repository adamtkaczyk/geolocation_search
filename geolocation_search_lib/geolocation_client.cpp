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

std::optional<GeoLocation> GeolocationClient::getIPGeoLocation(const std::string& ip)
{    
    std::string target = "/" + ip + "?access_key=" + api_key_;
    auto response = http_client_.request(methods::GET, uri_builder(U(ip)).append_query(U("access_key"), api_key_).to_string()).get();

    if (response.status_code() != 200) {
		throw std::runtime_error("Returned " + std::to_string(response.status_code()));
	}

    auto response_json = response.extract_json().get();

    return parseJson(response_json);
}

std::optional<GeoLocation> GeolocationClient::parseJson(json::value& json)
{
    if (!json.has_double_field("latitude")) {
        throw std::runtime_error("Field latitude not exist in json response");
    }

    if (!json.has_double_field("longitude")) {
        throw std::runtime_error("Field longitude not exist in json response");
    }

    return GeoLocation{json[U("latitude")].as_double(), json[U("longitude")].as_double()};
}
