#pragma once

#include <flatbuffers/flatbuffer_builder.h>
#include <nng/nng.h>
#include <nng/protocol/pair1/pair.h>

#include <cstdint>

#include "mocker/message_generated.h"

struct Client {
    Client(const char* url, nng_log_level level = NNG_LOG_DEBUG) {
        nng_pair1_open(&socket_);
        nng_dial(socket_, url, NULL, 0);

        nng_aio_alloc(&send_aio_, NULL, NULL);
        nng_aio_alloc(&recv_aio_, NULL, NULL);

        // set logger
        nng_log_set_logger(nng_stderr_logger);
        nng_log_set_level(level);
    }

    void sub(uint8_t market, uint8_t type, const char* symbol) {
        builder_.Clear();
        auto topic = Messages::CreateTopicDirect(builder_, market, type, symbol);
        auto topic_msg = Messages::CreateMessage(builder_, Messages::Payload::Topic, topic.Union());
        builder_.Finish(topic_msg);
        nng_msg* msg{};
        nng_msg_alloc(&msg, 0);
        nng_msg_append(msg, builder_.GetBufferPointer(), builder_.GetSize());
        nng_aio_set_msg(send_aio_, msg);
        nng_send_aio(socket_, send_aio_);
        nng_aio_wait(send_aio_);
    }

   private:
    nng_socket socket_{};
    nng_aio* send_aio_{};
    nng_aio* recv_aio_{};
    flatbuffers::FlatBufferBuilder builder_{1024};
};