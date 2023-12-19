#pragma once

#include <geolocation_client_interface.hpp>

#include <cpprest/http_client.h>
#include <cpprest/json.h>

class GeolocationClient: public GeolocationClientInterface {
public:
    GeolocationClient(const std::string host, const std::string api_key);
    ~GeolocationClient() = default;

    std::optional<GeoLocation> getIPGeoLocation(const std::string& ip) override final;
private:
    std::optional<GeoLocation> parseJson(web::json::value& json);

    web::http::client::http_client http_client_;
    const std::string api_key_;
};
