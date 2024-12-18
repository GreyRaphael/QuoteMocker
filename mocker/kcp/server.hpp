#pragma once

#include <flatbuffers/flatbuffer_builder.h>
#include <flatbuffers/verifier.h>
#include <fmtlog/fmtlog-inl.h>
#include <hv/EventLoop.h>
#include <hv/UdpServer.h>
#include <hv/hloop.h>
#include <hv/hsocket.h>
#include <hv/hstring.h>
#include <hv/htime.h>
#include <sys/socket.h>

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <hv/json.hpp>
#include <ranges>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "fmtlog/fmtlog.h"
#include "mocker/message_generated.h"
#include "mocker/random.hpp"

struct Client {
    sockaddr saddr;
    std::unordered_map<Messages::QuoteType, std::vector<std::string>> quote_map;
    uint64_t last_dt;
};

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

        // config logger
        fmtlog::setLogLevel(fmtlog::DBG);
        fmtlog::startPollingThread(200);
    }

    void start() {
        if (auto bindfd = server_.createsocket(port, host.c_str()); bindfd < 0) {
            loge("Failed to bind to {}:{}", host, port);
            return;
        }
        logi("server bind {}:{}", server_.host, server_.port);

        server_.onMessage = [this](const hv::SocketChannelPtr& channel, hv::Buffer* buf) {
            flatbuffers::Verifier verifier{(uint8_t*)buf->data(), buf->size()};
            if (!Messages::VerifyMessageBuffer(verifier)) {
                // invalid message
                logw("invalid messsage from {}, id={}", channel->peeraddr(), channel->id());
                builder_.Clear();
                auto reply = Messages::CreateErrorDataDirect(builder_, "Invalid Message");
                auto msg = Messages::CreateMessage(builder_, Messages::Payload::ErrorData, reply.Union());
                builder_.Finish(msg);
                channel->write(builder_.GetBufferPointer(), builder_.GetSize());
                return;
            }
            auto msg = Messages::GetMessage(buf->data());
            switch (msg->payload_type()) {
                case Messages::Payload::Subscribe: {
                    auto subscribes = msg->payload_as_Subscribe();
                    Client client{};
                    for (auto&& topic : *subscribes->topics()) {
                        auto symbols = topic->symbols()->string_view();
                        logd("onMessage Subscribe of {}", symbols);
                        auto range = symbols | std::views::split('|') | std::views::transform([](auto&& rng) { return std::string_view(rng.begin(), rng.end()); });
                        for (auto&& symbol : range) {
                            client.quote_map[topic->type()].emplace_back(symbol);
                        }
                    }

                    auto saddr = hio_peeraddr(channel->io());
                    memcpy(&client.saddr, saddr, sizeof(sockaddr));
                    client.last_dt = gettick_ms();
                    clients_[channel->peeraddr()] = client;
                    break;
                }
                case Messages::Payload::Replay: {
                    auto replay = msg->payload_as_Replay();
                    logd("onMessage Replay [{}, {}]", replay->dt_start(), replay->dt_end());
                    break;
                }
                case Messages::Payload::HeartBeat: {
                    clients_[channel->peeraddr()].last_dt = gettick_ms();
                    break;
                }
                default: {
                    logd("onMessage Unknown Payload Type");
                    break;
                }
            }
        };

        server_.onWriteComplete = [](const hv::SocketChannelPtr& channel, hv::Buffer* buf) {
        };

        // set kcp options
        server_.setKcp(&kcp_setting_);
        server_.start();

        // send Kline1d
        server_.loop()->setInterval(3000, [this](hv::TimerID timerID) {
            for (auto&& [url, client] : clients_) {
                if (client.quote_map.contains(Messages::QuoteType::K1d)) {
                    for (auto&& symbol : client.quote_map[Messages::QuoteType::K1d]) {
                        logd("send k1d:{} to {}", symbol, url);
                        builder_.Clear();
                        auto dt = gettick_ms();
                        auto k1d = Messages::CreateKlineDirect(
                            builder_,
                            symbol.c_str(),
                            dt,
                            amt_gen_(),
                            vol_gen_(),
                            num_gen_(),
                            price_gen_(),
                            price_gen_(),
                            price_gen_(),
                            price_gen_(),
                            price_gen_());
                        auto reply = Messages::CreateMessage(builder_, Messages::Payload::K1d, k1d.Union());
                        builder_.Finish(reply);
                        server_.sendto(builder_.GetBufferPointer(), builder_.GetSize(), &client.saddr);
                    }
                }
            }
        });

        // send Kline1min
        server_.loop()->setInterval(1000, [this](hv::TimerID timerID) {
            for (auto&& [url, client] : clients_) {
                if (client.quote_map.contains(Messages::QuoteType::K1min)) {
                    for (auto&& symbol : client.quote_map[Messages::QuoteType::K1min]) {
                        logd("send k1min:{} to {}", symbol, url);
                        builder_.Clear();
                        auto dt = gettick_ms();
                        auto k1min = Messages::CreateKlineDirect(
                            builder_,
                            symbol.c_str(),
                            dt,
                            amt_gen_(),
                            vol_gen_(),
                            num_gen_(),
                            price_gen_(),
                            price_gen_(),
                            price_gen_(),
                            price_gen_(),
                            price_gen_());
                        auto reply = Messages::CreateMessage(builder_, Messages::Payload::K1min, k1min.Union());
                        builder_.Finish(reply);
                        server_.sendto(builder_.GetBufferPointer(), builder_.GetSize(), &client.saddr);
                    }
                }
            }
        });

        // check alive
        server_.loop()->setInterval(4000, [this](hv::TimerID timerID) {
            logd("check alive clients");

            std::erase_if(clients_, [this](auto&& kv) {
                return kv.second.last_dt + 12000 < gettick_ms();
            });
        });
    }

    void
    wait() {
        while (getchar() != '\n');
    }

   public:
    int port;
    std::string host;

   private:
    kcp_setting_t kcp_setting_;
    hv::UdpServer server_;
    flatbuffers::FlatBufferBuilder builder_{1024};
    std::unordered_map<std::string, Client> clients_{};

   private:
    ClampedNormalIntGenerator vol_gen_{4000, 500};
    ClampedNormalIntGenerator amt_gen_{40000, 4000};
    ClampedNormalIntGenerator num_gen_{2000, 100};
    ClampedNormalIntGenerator price_gen_{};
};