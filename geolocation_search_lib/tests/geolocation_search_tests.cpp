#include <gtest/gtest.h>

#include <geolocation_search.hpp>

#include "database_client_mock.hpp"
#include "geolocation_client_mock.hpp"

using testing::_;

TEST(GeolocationSearch, getLocationFromDatabase)
{
    std::unique_ptr<DatabaseClientMock> database_client = std::make_unique<DatabaseClientMock>();
    std::unique_ptr<GeolocationClientMock> geolocation_client = std::make_unique<GeolocationClientMock>();

    GeoLocation geolocation{"example_host.com", "10.19.10.11", "ipv4", "NA", "North America", "US",
                            "United States", "CA", "California", "Mountain View", "94043", 11.11, 55.55};
    EXPECT_CALL(*database_client, getGeolocation(geolocation.host)).WillOnce(testing::Return(geolocation));

    GeolocationSearch geolocation_search{std::move(geolocation_client), std::move(database_client)};
    auto geolocation_return = geolocation_search.getIPGeoLocation(geolocation.host);

    ASSERT_TRUE(geolocation_return);
    EXPECT_NEAR(geolocation.latitude, geolocation_return->latitude, 0.001);
    EXPECT_NEAR(geolocation.longitude, geolocation_return->longitude, 0.001);
}

TEST(GeolocationSearch, getLocationFromNetwork)
{
    std::unique_ptr<DatabaseClientMock> database_client = std::make_unique<DatabaseClientMock>();
    std::unique_ptr<GeolocationClientMock> geolocation_client = std::make_unique<GeolocationClientMock>();

    GeoLocation geolocation{"example_host.com", "10.19.10.11", "ipv4", "NA", "North America", "US",
                            "United States", "CA", "California", "Mountain View", "94043", 11.11, 55.55};
    EXPECT_CALL(*database_client, getGeolocation(geolocation.host)).WillOnce(testing::Return(std::nullopt));
    EXPECT_CALL(*database_client, storeGeoLocation(_)).WillOnce(testing::Return(true));
    EXPECT_CALL(*geolocation_client, getIPGeoLocation(geolocation.host)).WillOnce(testing::Return(geolocation));

    GeolocationSearch geolocation_search{std::move(geolocation_client), std::move(database_client)};
    auto geolocation_return = geolocation_search.getIPGeoLocation(geolocation.host);

    ASSERT_TRUE(geolocation_return);
    EXPECT_NEAR(geolocation.latitude, geolocation_return->latitude, 0.001);
    EXPECT_NEAR(geolocation.longitude, geolocation_return->longitude, 0.001);
}

TEST(GeolocationSearch, deleteLocation)
{
    std::unique_ptr<DatabaseClientMock> database_client = std::make_unique<DatabaseClientMock>();
    std::unique_ptr<GeolocationClientMock> geolocation_client = std::make_unique<GeolocationClientMock>();

    GeoLocation geolocation{"example_host.com", "10.19.10.11", "ipv4", "NA", "North America", "US",
                            "United States", "CA", "California", "Mountain View", "94043", 11.11, 55.55};
    EXPECT_CALL(*database_client, getGeolocation(geolocation.host)).WillOnce(testing::Return(std::nullopt));
    EXPECT_CALL(*database_client, storeGeoLocation(_)).WillOnce(testing::Return(true));
    EXPECT_CALL(*database_client, deleteGeoLocation(geolocation.host)).WillOnce(testing::Return(true));
    EXPECT_CALL(*geolocation_client, getIPGeoLocation(geolocation.host)).WillOnce(testing::Return(geolocation));

    GeolocationSearch geolocation_search{std::move(geolocation_client), std::move(database_client)};
    auto geolocation_return = geolocation_search.getIPGeoLocation(geolocation.host);

    ASSERT_TRUE(geolocation_return);
    EXPECT_NEAR(geolocation.latitude, geolocation_return->latitude, 0.001);
    EXPECT_NEAR(geolocation.longitude, geolocation_return->longitude, 0.001);

    ASSERT_TRUE(geolocation_search.deleteLocation(geolocation.host));
}


TEST(GeolocationSearch, getLocationFromNetworkDeleteAndGetAgain)
{
    /*
    Call getGeolocation with empty db and empty cache should call getGeolocation, getIPGeoLocation from network and store result in db
    Call getGeolocation and result should be return from local cache
    Call deleteLocation which should call deleteGeoLocation from db client
    Call getGeolocation agin with the same result as first call, everything should be remove from cache
    */

    std::unique_ptr<DatabaseClientMock> database_client = std::make_unique<DatabaseClientMock>();
    std::unique_ptr<GeolocationClientMock> geolocation_client = std::make_unique<GeolocationClientMock>();

    GeoLocation geolocation{"example_host.com", "10.19.10.11", "ipv4", "NA", "North America", "US",
                            "United States", "CA", "California", "Mountain View", "94043", 11.11, 55.55};
    EXPECT_CALL(*database_client, getGeolocation(geolocation.host))
        .Times(2)
        .WillRepeatedly(testing::Return(std::nullopt));
    EXPECT_CALL(*database_client, storeGeoLocation(_))
        .Times(2)
        .WillRepeatedly(testing::Return(true));
    EXPECT_CALL(*database_client, deleteGeoLocation(geolocation.host)).WillOnce(testing::Return(true));
    EXPECT_CALL(*geolocation_client, getIPGeoLocation(geolocation.host)).Times(2).WillRepeatedly(testing::Return(geolocation));

    GeolocationSearch geolocation_search{std::move(geolocation_client), std::move(database_client)};
    auto geolocation_return = geolocation_search.getIPGeoLocation(geolocation.host);

    ASSERT_TRUE(geolocation_return);
    EXPECT_NEAR(geolocation.latitude, geolocation_return->latitude, 0.001);
    EXPECT_NEAR(geolocation.longitude, geolocation_return->longitude, 0.001);

    auto geolocation_second = geolocation_search.getIPGeoLocation(geolocation.host);

    ASSERT_TRUE(geolocation_return);
    EXPECT_NEAR(geolocation.latitude, geolocation_return->latitude, 0.001);
    EXPECT_NEAR(geolocation.longitude, geolocation_return->longitude, 0.001);

    ASSERT_TRUE(geolocation_search.deleteLocation(geolocation.host));

    geolocation_search.getIPGeoLocation(geolocation.host);
}
