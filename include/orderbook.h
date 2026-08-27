#include <map>
#include <deque>
#include <vector>

#include "order.h"
#include "trade.h"

class OrderBook {
public:
    uint64_t add_order(Order& order);
    double best_bid();
    double best_ask();
    std::vector<Trade> match_order(Order& order);

    void print_order_book() const;
    
private:
    std::map<double, std::deque<Order>> buy_orders;
    std::map<double, std::deque<Order>> sell_orders;
    uint64_t next_order_id = 0;
    uint64_t next_timestamp = 0;
};

// Pushes an order into the passed in order book (buy or sell) and assigns a timestamp to it
// Note that this function modifies the order's timestamp and id which it returns
uint64_t OrderBook::add_order(Order& order) {
    order.id = next_order_id;
    next_order_id++;

    order.timestamp = next_timestamp;
    next_timestamp++;

    if (order.is_buy) {
        auto& order_list = buy_orders[order.price];
        order_list.push_back(order);
    } else {
        auto& order_list = sell_orders[order.price];
        order_list.push_back(order);
    }

    return order.id;
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

std::vector<Trade> OrderBook::match_order(Order& order) {
    std::vector<Trade> trades;
    std::map<double, std::deque<Order>>& opposite_orders = order.is_buy ? sell_orders : buy_orders;
    
    while (order.quantity > 0 && opposite_orders.size() > 0) {
        double best_price_level = order.is_buy ? best_ask() : best_bid();

        bool price_condition = (order.is_buy && order.price >= best_price_level) || (!order.is_buy && order.price <= best_price_level);
        if (order.type == OrderType::Limit && !price_condition) {
            break;
        }

        Order& resting_order = opposite_orders[best_price_level].front();
        uint32_t trade_amount = std::min(order.quantity, resting_order.quantity);

        Trade trade{
            .price = resting_order.price,
            .quantity = trade_amount,
            .buy_order_id = order.is_buy ? order.id : resting_order.id,
            .sell_order_id = order.is_buy ? resting_order.id : order.id
        };
        trades.push_back(trade);

        order.quantity -= trade_amount;
        resting_order.quantity -= trade_amount;
        if (resting_order.quantity == 0) {
            opposite_orders[best_price_level].pop_front();

            if (opposite_orders[best_price_level].empty()) {
                opposite_orders.erase(best_price_level);
            }
        }
    }
    if (order.quantity > 0 && order.type == OrderType::Limit) {
        add_order(order);
    }
    // drop unfilled market orders

    return trades;
}