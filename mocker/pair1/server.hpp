#pragma once

#include <flatbuffers/flatbuffer_builder.h>
#include <nng/nng.h>
#include <nng/protocol/pair1/pair.h>

#include <cstddef>

#include "../message_generated.h"

struct Server {
    Server(const char* url, nng_log_level level = NNG_LOG_DEBUG) {
        nng_pair1_open(&socket_);
        nng_listen(socket_, url, NULL, 0);

        nng_aio_alloc(&send_aio_, NULL, NULL);
        nng_aio_alloc(&recv_aio_, NULL, NULL);

        // set logger
        nng_log_set_logger(nng_stderr_logger);
        nng_log_set_level(level);
    }

   private:
    nng_socket socket_{};
    nng_aio* send_aio_{};
    nng_aio* recv_aio_{};
    flatbuffers::FlatBufferBuilder builder_{1024};
};