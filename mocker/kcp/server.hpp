#pragma once

#include <fmt/core.h>
#include <hv/UdpServer.h>
#include <hv/hloop.h>

#include <cstdio>
#include <fstream>
#include <hv/json.hpp>

struct KcpServer {
    KcpServer(const char* jpath) {
        std::ifstream file{jpath};
        auto j = nlohmann::json::parse(file);
        // connection settings
        port = j["port"];
        // port < 0, use UDS
        host = port < 0 ? std::tmpnam(nullptr) : j["host"];

        // kcp settings
        kcp_setting_.conv = j["kcp"]["conv"];
        kcp_setting_.nodelay = j["kcp"]["nodelay"];
        kcp_setting_.interval = j["kcp"]["interval"];
        kcp_setting_.fastresend = j["kcp"]["resend"];
        kcp_setting_.nocwnd = j["kcp"]["nc"];
        kcp_setting_.sndwnd = j["kcp"]["sndwnd"];
        kcp_setting_.rcvwnd = j["kcp"]["rcvwnd"];
    }

    void start() {
        if (auto bindfd = server_.createsocket(port, host.c_str()); bindfd < 0) {
            fmt::println("Failed to bind to {}:{}", host, port);
            return;
        }
        fmt::println("server bind {}:{}", server_.host, server_.port);

        server_.onMessage = [](const hv::SocketChannelPtr& channel, hv::Buffer* buf) {

        };

        server_.onWriteComplete = [](const hv::SocketChannelPtr& channel, hv::Buffer* buf) {

        };

        // set kcp options
        server_.setKcp(&kcp_setting_);
        server_.start();
    }

    void wait() {
        while (getchar() != '\n');
    }

   public:
    int port;
    std::string host;

   private:
    kcp_setting_t kcp_setting_;
    hv::UdpServer server_;
};