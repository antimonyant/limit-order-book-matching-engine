#pragma once

#include <cstdint>

struct Trade {
    double price;       // Trades should still execute based on the price of the resting order in the order book
    uint32_t quantity;
    uint64_t buy_order_id;
    uint64_t sell_order_id;
};