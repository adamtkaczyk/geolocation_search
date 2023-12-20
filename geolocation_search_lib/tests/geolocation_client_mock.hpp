#pragma once

#include <geolocation_client_interface.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

class GeolocationClientMock : public GeolocationClientInterface {
public:
    MOCK_METHOD(std::optional<GeoLocation>, getIPGeoLocation, (const std::string& host), ());
};
