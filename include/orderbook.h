#include <map>
#include <deque>
#include "order.h"

class OrderBook {
public:
    void add_order(Order& order);
    double best_bid();
    double best_ask();

    void print_order_book() const;
    
private:
    std::map<double, std::deque<Order>> buy_orders;
    std::map<double, std::deque<Order>> sell_orders;
    uint64_t next_timestamp = 0;
};

// Pushes an order into the passed in order book (buy or sell) and assigns a timestamp to it
// Note that this function modifies the order's timestamp
void OrderBook::add_order(Order& order) {
    order.timestamp = next_timestamp;
    next_timestamp++;

    if (order.is_buy) {
        auto& order_list = buy_orders[order.price];
        order_list.push_back(order);
    } else {
        auto& order_list = sell_orders[order.price];
        order_list.push_back(order);
    }
}

double OrderBook::best_bid() {
    // No bids available
    if (buy_orders.empty()) {
        return -1.0;
    }
    // Return the highest bid price
    return buy_orders.rbegin()->first; 
}

double OrderBook::best_ask() {
    // No asks available
    if (sell_orders.empty()) {
        return -1.0;
    }
    // Return the lowest ask price
    return sell_orders.begin()->first; 
}

void OrderBook::print_order_book() const {
    std::cout << "Buy Orders:\n";
    for (auto it = buy_orders.rbegin(); it != buy_orders.rend(); ++it) {
        const auto& [price, orders] = *it;
        for (const auto& order : orders) {
            std::cout << "ID: " << order.id << ", Price: " << price << ", Quantity: " << order.quantity << ", Timestamp: " << order.timestamp << "\n";
        }
    }

    std::cout << "Sell Orders:\n";
    for (const auto& [price, orders] : sell_orders) {
        for (const auto& order : orders) {
            std::cout << "ID: " << order.id << ", Price: " << price << ", Quantity: " << order.quantity << ", Timestamp: " << order.timestamp << "\n";
        }
    }
}