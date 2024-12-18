#pragma once

#include <flatbuffers/flatbuffer_builder.h>
#include <nng/nng.h>
#include <nng/protocol/pair1/pair.h>
#include <nng/supplemental/util/platform.h>

#include <cstdint>

#include "../message_generated.h"

struct Client {
    Client(const char* url, nng_log_level level = NNG_LOG_DEBUG) {
        nng_pair1_open_poly(&socket_);
        nng_dial(socket_, url, NULL, 0);

        nng_aio_alloc(&recv_aio_, recv_callback, this);

        // set logger
        nng_log_set_logger(nng_stderr_logger);
        nng_log_set_level(level);
    }

    ~Client() {
        nng_aio_free(recv_aio_);
        nng_close(socket_);
    }

    void sub(uint8_t market, uint8_t type, const char* symbol) {
        builder_.Clear();
        auto topic = Messages::CreateTopicDirect(builder_, market, type, symbol);
        auto topic_msg = Messages::CreateMessage(builder_, Messages::Payload::Topic, topic.Union());
        builder_.Finish(topic_msg);

        nng_send(socket_, builder_.GetBufferPointer(), builder_.GetSize(), 0);
        nng_log_debug("CLIENT", "client send sub successfully");

        // Start receiving messages
        nng_recv_aio(socket_, recv_aio_);
    }

   private:
    nng_socket socket_{};
    nng_aio* recv_aio_{};
    flatbuffers::FlatBufferBuilder builder_{1024};

   private:
    static void recv_callback(void* arg) {
        auto client = static_cast<Client*>(arg);
        client->on_recv();
    }

    void on_recv() {
        auto msg = nng_aio_get_msg(recv_aio_);

        auto msg_data = nng_msg_body(msg);
        auto msg_size = nng_msg_len(msg);

        auto ptr = Messages::GetMessage(msg_data);
        switch (ptr->payload_type()) {
            case Messages::Payload::EtfBar1d: {
                auto bar = ptr->payload_as_EtfBar1d();
                nng_log_debug("CLIENT", "recv bar1d %s, amount=%d", bar->symbol()->c_str(), bar->amount());
                break;
            }
            default: {
                nng_log_debug("CLIENT", "Unknown payload type, len=%d, content=%s", msg_size, (char*)msg_data);
                break;
            }
        }

        nng_msg_free(msg);
        nng_msleep(500);
        nng_recv_aio(socket_, recv_aio_);
    }
};