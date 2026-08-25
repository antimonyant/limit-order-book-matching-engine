#pragma once

#include <cstdint>


struct Order {
    uint64_t id;
    double price;         // ignored for market orders
    uint32_t quantity;    // remaining, unfilled quantity
    bool is_buy;          // true for buy, false for sell
    uint64_t timestamp;   // monotonically increasing arrival order, used for
                          //    time priority within a price level
};