#include <gtest/gtest.h>

#include "http_test_server.hpp"
#include <geolocation_search.hpp>
#include <thread>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

TEST(GeolocationClient, getIPGeoLocation)
{
    HttpTestServer server{"127.0.0.1", 7000};
    server.setGetHandeler([](http_request request) {
        auto answer = json::value::object();
        answer["latitude"] = json::value::number(34.0656);
        answer["longitude"] = json::value::number(-118.241);

        request.reply(200, answer);
    });
    std::thread th(&HttpTestServer::run, &server);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::string host = "http://localhost:7000";
    const std::string ip = "134.201.250.155";
    GeolocationClient geolocation_client{host, "123456"};
    auto geolocation = geolocation_client.getIPGeoLocation(ip);

    server.stop();
    th.join();

    ASSERT_TRUE(geolocation);
    EXPECT_NEAR(34.0656, geolocation->latitude, 0.001);
    EXPECT_NEAR(-118.241, geolocation->longitude, 0.001);
}

TEST(GeolocationClient, getIPGeoLocationIncorrectJson)
{
    HttpTestServer server{"127.0.0.1", 7000};
    server.setGetHandeler([](http_request request) {
        auto answer = json::value::object();
        answer["other"] = json::value::string("test");

        request.reply(200, answer);
    });
    std::thread th(&HttpTestServer::run, &server);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::string host = "http://localhost:7000";
    const std::string ip = "134.201.250.155";
    GeolocationClient geolocation_client{host, "123456"};
    EXPECT_THROW(geolocation_client.getIPGeoLocation(ip), std::runtime_error);

    server.stop();
    th.join();
}

TEST(GeolocationClient, getIPGeoLocationEmptyJson)
{
    HttpTestServer server{"127.0.0.1", 7000};
    server.setGetHandeler([](http_request request) {
        request.reply(200);
    });
    std::thread th(&HttpTestServer::run, &server);
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::string host = "http://localhost:7000";
    const std::string ip = "134.201.250.155";
    GeolocationClient geolocation_client{host, "123456"};
    EXPECT_THROW(geolocation_client.getIPGeoLocation(ip), std::runtime_error);

    server.stop();
    th.join();
}
