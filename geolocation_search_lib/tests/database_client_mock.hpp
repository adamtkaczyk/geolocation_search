#pragma once

#include <databse_client_interface.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

class DatabaseClientMock : public DatabaseClientInterface {
public:
    MOCK_METHOD(bool, store, (const GeoLocation& geolocation), ());
    MOCK_METHOD(std::optional<GeoLocation>, getGeolocation, (const std::string& host), ());
};
