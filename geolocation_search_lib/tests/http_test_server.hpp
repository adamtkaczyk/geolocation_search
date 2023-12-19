#pragma once

#include <cpprest/http_listener.h>

#include <optional>

using HttpHandler = std::function<void(web::http::http_request request)>;

class HttpTestServer {
public:
    HttpTestServer(const std::string host, const int port);

    void run();
    void stop();

    void setGetHandeler(HttpHandler handler) {
        http_get_handler_ = handler;
    }

    void setPostHandeler(HttpHandler handler) {
        http_post_handler_ = handler;
    }

    void setPutHandeler(HttpHandler handler) {
        http_put_handler_ = handler;
    }

    void setDelHandeler(HttpHandler handler) {
        http_del_handler_ = handler;
    }
private:
    bool finished_ = false;
    const std::string host_;
    const int port_;

    std::optional<HttpHandler> http_get_handler_;
    std::optional<HttpHandler> http_post_handler_;
    std::optional<HttpHandler> http_put_handler_;
    std::optional<HttpHandler> http_del_handler_;
};
