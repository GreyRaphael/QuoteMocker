#pragma once

#include <flatbuffers/flatbuffer_builder.h>
#include <fmt/core.h>
#include <hv/EventLoop.h>
#include <hv/UdpServer.h>
#include <hv/hloop.h>
#include <hv/htime.h>

#include <chrono>
#include <cstdio>
#include <fstream>
#include <hv/json.hpp>
#include <thread>

#include "mocker/message_generated.h"

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

        server_.onMessage = [this](const hv::SocketChannelPtr& channel, hv::Buffer* buf) {
            auto msg = Messages::GetMessage(buf->data());
            switch (msg->payload_type()) {
                case Messages::Payload::Topic: {
                    auto topic = msg->payload_as_Topic();
                    fmt::println("onMessage Topic of {}, mkt={},type={},symbol={}", channel->peeraddr(), topic->market(), topic->type(), topic->symbol()->c_str());

                    channel->setHeartbeat(1000, [&channel] {
                        fmt::println("heartbeat, {}", channel->peeraddr());
                    });

                    server_.loop()->setInterval(1000, [this, channel](hv::TimerID timerID) {
                        builder_.Clear();
                        auto dt = gettick_ms();
                        auto bar1d = Messages::CreateBarDataDirect(
                            builder_,
                            "600000",
                            dt,
                            25000,
                            1000,
                            200,
                            1230,
                            1231,
                            1238,
                            1229,
                            1235);
                        auto reply = Messages::CreateMessage(builder_, Messages::Payload::EtfBar1d, bar1d.Union());
                        builder_.Finish(reply);
                        // server_.sendto(builder_.GetBufferPointer(), builder_.GetSize());
                        channel->write(builder_.GetBufferPointer(), builder_.GetSize());
                    });

                    break;
                }
                default: {
                    fmt::println("onMessage Unknown Payload Type");
                    break;
                }
            }
        };

        server_.onWriteComplete = [](const hv::SocketChannelPtr& channel, hv::Buffer* buf) {
            // fmt::println("onWriteComplete: {} bytes", buf->size());
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
    flatbuffers::FlatBufferBuilder builder_{1024};
};