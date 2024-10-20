#include <nng/nng.h>
#include <nng/protocol/pubsub0/pub.h>
#include <nng/supplemental/util/platform.h>

#include <cstring>

#include "quotetypes.h"
#include "random.hpp"

struct Publisher {
    Publisher(const char* url, nng_log_level level = NNG_LOG_DEBUG) {
        nng_pub0_open(&socket_);
        nng_listen(socket_, url, NULL, 0);
        // set logger
        nng_log_set_logger(nng_stderr_logger);
        nng_log_set_level(level);
    }

    int publish(void* data, size_t size) const {
        return nng_send(socket_, data, size, 0);
    }

   private:
    nng_socket socket_{};
};

inline void publish_bars(Publisher const& publisher, std::vector<std::string> const& symbols, int interval = 1000) {
    int timestamp{};
    Bar bar{};
    ClampedNormalIntGenerator gen{100000, 4000};
    while (true) {
        timestamp = nng_clock();
        for (auto&& symbol : symbols) {
            memcpy(bar.symbol, symbol.data(), symbol.size());
            bar.timestamp = timestamp;
            bar.close = gen();

            int ret = publisher.publish(&bar, sizeof(Bar));
            if (0 == ret) {
                nng_log_debug("BAR", "send bar %s at %d", bar.symbol, timestamp);
            } else {
                nng_log_err("BAR", "send failed with error:%s", nng_strerror(ret));
            }
        }
        nng_msleep(interval);
    }
}

inline void publish_ticks(Publisher const& publisher, std::vector<std::string> const& symbols, int interval = 1000) {
    int timestamp{};
    Tick tick{};
    ClampedNormalIntGenerator gen{100000, 4000};
    while (true) {
        timestamp = nng_clock();
        for (auto&& symbol : symbols) {
            memcpy(tick.symbol, symbol.data(), symbol.size());
            tick.timestamp = timestamp;
            tick.close = gen();

            int ret = publisher.publish(&tick, sizeof(Tick));
            if (0 == ret) {
                nng_log_debug("TICK", "send tick %s at %d", tick.symbol, timestamp);
            } else {
                nng_log_err("TICK", "send failed with error:%s", nng_strerror(ret));
            }
        }
        nng_msleep(interval);
    }
}