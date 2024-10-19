#include <nng/nng.h>
#include <nng/protocol/pubsub0/pub.h>
#include <nng/supplemental/util/platform.h>

#include <cstring>
#include <format>

#include "quotetypes.h"
#include "random.hpp"

struct Publisher {
    Publisher(const char* url) {
        nng_pub0_open(&socket_);
        nng_listen(socket_, url, NULL, 0);
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
                nng_log_debug("BAR", std::format("send bar at {}\n", timestamp).c_str());
            } else {
                nng_log_warn("BAR", std::format("send failed with error: {}\n", nng_strerror(ret)).c_str());
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
                nng_log_debug("TICK", std::format("send tick at {}\n", timestamp).c_str());
            } else {
                nng_log_warn("TICK", std::format("send failed with error: {}\n", nng_strerror(ret)).c_str());
            }
        }
        nng_msleep(interval);
    }
}