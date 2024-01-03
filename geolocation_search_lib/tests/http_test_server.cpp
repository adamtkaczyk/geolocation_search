#include "http_test_server.hpp"

#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <chrono>
#include <thread>
#include <spdlog/spdlog.h>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

HttpTestServer::HttpTestServer(const std::string host, const int port)
    : host_(host)
    , port_(port)
{
}

void HttpTestServer::run()
{
    const std::string address = "http://" + host_ + ":" + std::to_string(port_);
    spdlog::info("Start endpoint: {}", address.c_str());

    http_listener listener(address);

    if(http_get_handler_) {
        listener.support(methods::GET, *http_get_handler_);
    }

    if(http_post_handler_) {
        listener.support(methods::POST, *http_post_handler_);
    }

    if(http_put_handler_) {
        listener.support(methods::PUT, *http_put_handler_);
    }

    if(http_del_handler_) {
        listener.support(methods::DEL, *http_del_handler_);
    }

    try {
        listener
            .open()
            .then([&listener]() {
                spdlog::info("starting to listen");
            })
            .wait();
        while (!finished_) {
            std::this_thread::sleep_for(std::chrono::seconds(3));
        };
    } catch (std::exception const & e) {
        spdlog::info("Exception: {}", e.what());
    }
}

void HttpTestServer::stop()
{
    finished_ = true;
}

