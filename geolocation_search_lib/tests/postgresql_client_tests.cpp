#include <gtest/gtest.h>

#include <postgresql_client.hpp>

TEST(PostgreSQLClient, storeGeoLocation)
{
    GeoLocation geolocation{"example_host.com", "10.19.10.11", "ipv4", "NA", "North America", "US",
                            "United States", "CA", "California", "Mountain View", "94043", 1.234, 56.1123};

    const std::string db_name = "geolocations";
    const std::string db_user = "postgres";
    const std::string db_password = "testpassword123";
    const std::string db_hostname = "127.0.0.1";
    const unsigned db_port = 5432;
    PostgreSQLClient postgresql_clinet{db_name, db_user, db_password, db_hostname, db_port};
    postgresql_clinet.connect();

    EXPECT_TRUE(postgresql_clinet.storeGeoLocation(geolocation));
    auto geolocation_from_db = postgresql_clinet.getGeolocation(geolocation.host);
    ASSERT_TRUE(geolocation_from_db);
    EXPECT_EQ(geolocation.host, geolocation_from_db->host);
    EXPECT_EQ(geolocation.latitude, geolocation_from_db->latitude);
    EXPECT_EQ(geolocation.longitude, geolocation_from_db->longitude);
    EXPECT_TRUE(postgresql_clinet.deleteGeoLocation(geolocation.host));
}
