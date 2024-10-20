#include <mocker/quotetypes.h>
#include <nng/nng.h>
#include <nng/protocol/pubsub0/sub.h>

#include <memory>
#include <string>
#include <vector>

struct Subscriber {
    Subscriber(const char* url, std::vector<std::string> const& topics, nng_log_level level = NNG_LOG_DEBUG) {
        nng_sub0_open(&socket_);
        for (auto&& topic : topics) {
            nng_socket_set(socket_, NNG_OPT_SUB_SUBSCRIBE, topic.c_str(), topic.size());
        }
        nng_dial(socket_, url, NULL, 0);
        // set logger
        nng_log_set_logger(nng_stderr_logger);
        nng_log_set_level(level);
    }
    ~Subscriber() { nng_close(socket_); }

    int receive(void* data, size_t* size) const {
        return nng_recv(socket_, data, size, NNG_FLAG_ALLOC);
    }

   private:
    nng_socket socket_{};
};

inline void subscribe_bars(Subscriber const& subscriber) {
    auto ptr = std::make_shared<Bar>();
    size_t size{};
    while (true) {
        int ret = subscriber.receive(&ptr, &size);
        if (0 == ret) {
            nng_log_debug("SUB", "recv data %s at %d", ptr->symbol, ptr->timestamp);
        } else {
            nng_log_err("SUB", "recv failed with error:%s", nng_strerror(ret));
        }
    }
}

inline void subscribe_ticks(Subscriber const& subscriber) {
    auto ptr = std::make_shared<Tick>();
    size_t size{};
    while (true) {
        int ret = subscriber.receive(&ptr, &size);
        if (0 == ret) {
            nng_log_debug("SUB", "recv data %s at %d", ptr->symbol, ptr->timestamp);
        } else {
            nng_log_err("SUB", "recv failed with error:%s", nng_strerror(ret));
        }
    }
}