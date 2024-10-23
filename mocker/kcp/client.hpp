#pragma once
#include <flatbuffers/flatbuffer_builder.h>
#include <fmt/core.h>
#include <hv/UdpClient.h>

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <hv/json.hpp>
#include <vector>

#include "mocker/message_generated.h"

struct KcpClient {
    KcpClient(const char* jpath) {
        std::ifstream file{jpath};
        auto j = nlohmann::json::parse(file);
        // connection settings
        remote_host = j["host"];
        remote_port = j["port"];
        // kcp settings
        kcp_setting_.conv = j["kcp"]["conv"];
        kcp_setting_.nodelay = j["kcp"]["nodelay"];
        kcp_setting_.interval = j["kcp"]["interval"];
        kcp_setting_.fastresend = j["kcp"]["resend"];
        kcp_setting_.nocwnd = j["kcp"]["nc"];
        kcp_setting_.sndwnd = j["kcp"]["sndwnd"];
        kcp_setting_.rcvwnd = j["kcp"]["rcvwnd"];
        // local host for UDS
        local_host = std::tmpnam(nullptr);
    }
    ~KcpClient() {
        std::filesystem::remove(local_host);
    }

    void start() {
        if (auto sockfd = client_.createsocket(remote_port, remote_host.c_str()); sockfd < 0) {
            fmt::println("Failed to create socket {}:{}", remote_host, remote_port);
            return;
        }

        // deal wit UDS path
        if (remote_port < 0) {
            client_.bind(remote_port, local_host);
        }

        client_.onMessage = [](const hv::SocketChannelPtr& channel, hv::Buffer* buf) {
            auto msg = Messages::GetMessage(buf->data());
            switch (msg->payload_type()) {
                case Messages::Payload::EtfBar1d: {
                    auto etf_bar1d = msg->payload_as_EtfBar1d();
                    fmt::println("EtfBar1d: {},vol={}", etf_bar1d->symbol()->c_str(), etf_bar1d->volume());
                    break;
                }
                case Messages::Payload::EtfBar1min: {
                    auto etf_bar1min = msg->payload_as_EtfBar1min();
                    fmt::println("EtfBar1min: {},vol={}", etf_bar1min->symbol()->c_str(), etf_bar1min->volume());
                    break;
                }
                case Messages::Payload::EtfBar1w: {
                    auto etf_bar1w = msg->payload_as_EtfBar1w();
                    fmt::println("EtfBar1w: {},vol={}", etf_bar1w->symbol()->c_str(), etf_bar1w->volume());
                    break;
                }
                case Messages::Payload::EtfBar1mon: {
                    auto etf_bar1mon = msg->payload_as_EtfBar1mon();
                    fmt::println("EtfBar1mon: {},vol={}", etf_bar1mon->symbol()->c_str(), etf_bar1mon->volume());
                    break;
                }
                default: {
                    break;
                }
            }
        };
        client_.onWriteComplete = [](const hv::SocketChannelPtr& channel, hv::Buffer* buf) {

        };

        // set kcp options
        client_.setKcp(&kcp_setting_);
        client_.start();
    }

    void subscribe(std::vector<std::string> const& symbols) {
        flatbuffers::FlatBufferBuilder builder;
        auto topic = Messages::CreateTopicDirect(builder, 1, 1, "600000");
        auto msg = Messages::CreateMessage(builder, Messages::Payload::Topic, topic.Union());
        builder.Finish(msg);
        client_.sendto(builder.GetBufferPointer(), builder.GetSize());
    }

    void wait() {
        while (getchar() != '\n');
    }

   public:
    int remote_port;
    std::string remote_host;
    const char* local_host;

   private:
    kcp_setting_t kcp_setting_;
    hv::UdpClient client_;
};