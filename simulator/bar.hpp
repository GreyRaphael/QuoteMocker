#pragma once

#include <cstdint>

struct Bar {
    char symbol[8];
    int64_t timestamp;
    uint64_t volume;
    uint64_t amount;
    uint32_t open;
    uint32_t high;
    uint32_t low;
    uint32_t close;
    uint32_t num_trades;
};