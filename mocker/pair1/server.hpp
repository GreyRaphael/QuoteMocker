#pragma once

#include <flatbuffers/flatbuffer_builder.h>
#include <nng/nng.h>
#include <nng/protocol/pair1/pair.h>
#include <nng/supplemental/util/platform.h>

#include <cstddef>

#include "../message_generated.h"

struct Server {
    Server(const char* url, nng_log_level level = NNG_LOG_DEBUG) {
        // nng_pair1_open(&socket_);
        nng_pair1_open_poly(&socket_);
        nng_listen(socket_, url, NULL, 0);

        nng_aio_alloc(&send_aio_, NULL, NULL);
        nng_aio_alloc(&recv_aio_, recv_callback, this);

        // set logger
        nng_log_set_logger(nng_stderr_logger);
        nng_log_set_level(level);
    }

    void run() {
        nng_recv_aio(socket_, recv_aio_);
    }

   private:
    nng_socket socket_{};
    nng_aio* send_aio_{};
    nng_aio* recv_aio_{};
    flatbuffers::FlatBufferBuilder builder_{1024};

   private:
    static void recv_callback(void* arg) {
        auto server = static_cast<Server*>(arg);
        server->on_recv();
    }

    void on_recv() {
        auto msg = nng_aio_get_msg(recv_aio_);
        auto pipe = nng_msg_get_pipe(msg);
        nng_log_warn("SERVER", "pipe %d connected", pipe.id);

        auto msg_data = nng_msg_body(msg);
        auto msg_size = nng_msg_len(msg);

        auto ptr = Messages::GetMessage(msg_data);
        switch (ptr->payload_type()) {
            case Messages::Payload::Topic: {
                auto topic = ptr->payload_as_Topic();
                nng_log_debug("SERVER", "recv topic %s, market=%d", topic->symbol()->c_str(), topic->market());
                auto timestamp = nng_clock();
                int index = 0;
                while (true) {
                    builder_.Clear();
                    auto bar = Messages::CreateBarDataDirect(
                        builder_,
                        topic->symbol()->c_str(),
                        timestamp,
                        index++);
                    auto reply = Messages::CreateMessage(builder_, Messages::Payload::EtfBar1d, bar.Union());
                    builder_.Finish(reply);
                    send_reply(pipe, builder_.GetBufferPointer(), builder_.GetSize());
                }

                break;
            }
            default: {
                nng_log_debug("SERVER", "Unknown payload type, len=%d, content=%s", msg_size, (char*)msg_data);
                break;
            }
        }
        nng_msg_free(msg);
        // Start receiving the next message
        nng_recv_aio(socket_, recv_aio_);
    }

    void send_reply(nng_pipe pipe, void* data, size_t size) {
        nng_msg* reply{};
        nng_msg_alloc(&reply, 0);
        nng_msg_append(reply, data, size);
        nng_msg_set_pipe(reply, pipe);
        nng_aio_set_msg(send_aio_, reply);
        nng_send_aio(socket_, send_aio_);
        nng_aio_wait(send_aio_);
    }
};