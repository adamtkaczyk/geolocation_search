#pragma once

#include <databse_client_interface.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

class DatabaseClientMock : public DatabaseClientInterface {
public:
    MOCK_METHOD(void, connect, (), ());
    MOCK_METHOD(bool, storeGeoLocation, (const GeoLocation& geolocation), ());
    MOCK_METHOD(bool, deleteGeoLocation, (const std::string& host), ());
    MOCK_METHOD(std::optional<GeoLocation>, getGeolocation, (const std::string& host), ());
};
