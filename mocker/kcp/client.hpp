#pragma once
#include <flatbuffers/flatbuffer_builder.h>
#include <fmtlog/fmtlog-inl.h>
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

        // config logger
        fmtlog::setLogLevel(fmtlog::DBG);
        fmtlog::startPollingThread(200);
    }
    ~KcpClient() {
        std::filesystem::remove(local_host);
    }

    void start() {
        if (auto sockfd = client_.createsocket(remote_port, remote_host.c_str()); sockfd < 0) {
            loge("Failed to create socket {}:{}", remote_host, remote_port);
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
                    logd("EtfBar1d: {},vol={}", etf_bar1d->symbol()->c_str(), etf_bar1d->volume());
                    break;
                }
                case Messages::Payload::EtfBar1min: {
                    auto etf_bar1min = msg->payload_as_EtfBar1min();
                    logd("EtfBar1min: {},vol={}", etf_bar1min->symbol()->c_str(), etf_bar1min->volume());
                    break;
                }
                case Messages::Payload::EtfBar1w: {
                    auto etf_bar1w = msg->payload_as_EtfBar1w();
                    logd("EtfBar1w: {},vol={}", etf_bar1w->symbol()->c_str(), etf_bar1w->volume());
                    break;
                }
                case Messages::Payload::EtfBar1mon: {
                    auto etf_bar1mon = msg->payload_as_EtfBar1mon();
                    logd("EtfBar1mon: {},vol={}", etf_bar1mon->symbol()->c_str(), etf_bar1mon->volume());
                    break;
                }
                default: {
                    logd("Unknown payload type");
                    break;
                }
            }
        };
        client_.onWriteComplete = [](const hv::SocketChannelPtr& channel, hv::Buffer* buf) {

        };

        // set kcp options
        client_.setKcp(&kcp_setting_);
        client_.start();

        // send heartbeat periodically
        client_.loop()->setInterval(5000, [this](hv::TimerID timerID) {
            builder_.Clear();
            auto hb = Messages::CreateHeartBeat(builder_);
            auto msg = Messages::CreateMessage(builder_, Messages::Payload::HeartBeat, hb.Union());
            builder_.Finish(msg);
            client_.sendto(builder_.GetBufferPointer(), builder_.GetSize());
            logd("HeartBeat sent {} bytes", builder_.GetSize());
        });
    }

    void subscribe(std::vector<std::string> const& symbols) {
        builder_.Clear();
        auto topic = Messages::CreateTopicDirect(builder_, 1, 1, "300116");
        auto msg = Messages::CreateMessage(builder_, Messages::Payload::Topic, topic.Union());
        builder_.Finish(msg);
        client_.sendto(builder_.GetBufferPointer(), builder_.GetSize());
    }

    void replay(std::vector<std::string> const& symbols) {
        builder_.Clear();
        auto topic = Messages::CreateTopicDirect(builder_, 1, 1, "688538");
        auto replay = Messages::CreateReplay(builder_, 1729390224, 1729735824, topic);
        auto msg = Messages::CreateMessage(builder_, Messages::Payload::Replay, replay.Union());
        builder_.Finish(msg);
        client_.sendto(builder_.GetBufferPointer(), builder_.GetSize());
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
    flatbuffers::FlatBufferBuilder builder_{1024};
};