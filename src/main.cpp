#include <iostream>

#include "order.h"
#include "orderbook.h"


int main() {
    Order buy1{1, 50.10, 100, true, 0};
    Order buy2{2, 50.10, 100, true, 0};
    Order buy3{3, 48.6, 30, true, 0};
    Order buy4{3, 51.32, 40, true, 0};

    Order sell1{1, 50.00, 100, false, 0};
    Order sell2{2, 50.00, 100, false, 0};
    Order sell3{3, 49.12, 30, false, 0};
    Order sell4{3, 52.33, 40, false, 0};

    OrderBook order_book;
    order_book.add_order(buy1);
    order_book.add_order(sell1);
    order_book.add_order(buy3);
    order_book.add_order(sell4);
    order_book.add_order(sell3);
    order_book.add_order(buy4);
    order_book.add_order(buy2);
    order_book.add_order(sell2);

    std::cout << "Scaffold compiles.\n";
    std::cout << "Sample order: id=" << buy1.id
              << " side=" << (buy1.is_buy ? "BUY" : "SELL")
              << " price=" << buy1.price
              << " qty=" << buy1.quantity << "\n";

    std::cout << "Best bid: " << order_book.best_bid() << "\n";
    std::cout << "Best ask: " << order_book.best_ask() << "\n";
    order_book.print_order_book();
    return 0;
}