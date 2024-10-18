#include <nng/nng.h>
#include <nng/protocol/pubsub0/pub.h>
#include <nng/supplemental/util/platform.h>

#include <cstring>

#include "quotetypes.h"
#include "random.hpp"

inline void publish_bars(const char* url, std::vector<std::string> const& symbols, int interval = 1000) {
    nng_socket socket{};
    nng_pub0_open(&socket);
    nng_listen(socket, url, NULL, 0);

    int timestamp{};
    Bar bar{};
    ClampedNormalIntGenerator gen{100000, 4000};  // mean, stddev
    while (true) {
        timestamp = nng_clock();
        for (auto const& symbol : symbols) {
            memcpy(bar.symbol, symbol.data(), symbol.size());
            bar.timestamp = timestamp;
            bar.close = gen();

            nng_send(socket, &bar, sizeof(Bar), 0);
            printf("nng send bar %s at %lu\n", bar.symbol, bar.timestamp);
        }
        nng_msleep(interval);
    }
}

inline void publish_ticks(const char* url, std::vector<std::string> const& symbols, int interval = 1000) {
    nng_socket socket{};
    nng_pub0_open(&socket);
    nng_listen(socket, url, NULL, 0);

    int timestamp{};
    Tick tick{};
    ClampedNormalIntGenerator gen{100000, 4000};  // mean, stddev
    while (true) {
        timestamp = nng_clock();
        for (auto const& symbol : symbols) {
            memcpy(tick.symbol, symbol.data(), symbol.size());
            tick.timestamp = timestamp;
            tick.close = gen();

            nng_send(socket, &tick, sizeof(Tick), 0);
            printf("nng send tick %s at %lu\n", tick.symbol, tick.timestamp);
        }
        nng_msleep(interval);
    }
}