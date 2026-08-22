#include <iostream>

#include "order.h"


int main() {
    Order sample{1, 50.10, 100, true, 0};

    std::cout << "Scaffold compiles.\n";
    std::cout << "Sample order: id=" << sample.id
              << " side=" << (sample.is_buy ? "BUY" : "SELL")
              << " price=" << sample.price
              << " qty=" << sample.quantity << "\n";

    return 0;
}