#include <gtest/gtest.h>
#include "OrderBook.h"

// gtest_main (linked via CMake) supplies the main function which automatically finds and 
//  runs every TEST block below.

TEST(MatchingTest, SingleFullFill) {
    OrderBook book;
    // TODO: add one resting sell order
    Order sell1{1, 50.00, 100, false, 0, OrderType::Limit};
    Order sell2{2, 67.91, 42, false, 1, OrderType::Limit};
    book.add_order(sell1);
    book.add_order(sell2);
    book.print_order_book();
    // Match a LIMIT buy that exactly matches its quantity
    Order buyLimit{1, 50.00, 100, true, 0, OrderType::Limit};
    auto trades = book.match_order(buyLimit);

    // assert first so that trades[0] is valid
    ASSERT_EQ(trades.size(), 1);
    EXPECT_EQ(trades[0].quantity, 100);
    EXPECT_DOUBLE_EQ(trades[0].price, 50.00);
    // check the resting side
    EXPECT_EQ(book.best_ask(), 67.91);

    // Match a MARKET buy that exactly matches its quantity
    Order buyMarket{2, 71.00, 42, true, 1, OrderType::Market};
    trades = book.match_order(buyMarket);
    ASSERT_EQ(trades.size(), 1);
    EXPECT_EQ(trades[0].quantity, 42);
    EXPECT_DOUBLE_EQ(trades[0].price, 67.91);
    EXPECT_EQ(book.best_ask(), -1.0);
}

TEST(MatchingTest, PartialFill) {
    OrderBook book;
    // Resting sell with MORE quantity than the incoming order
    Order sell1{1, 50.00, 100, false, 0, OrderType::Limit};
    book.add_order(sell1);

    Order buyLimit{1, 50.00, 36, true, 0, OrderType::Limit};
    auto trades = book.match_order(buyLimit);

    ASSERT_EQ(trades.size(), 1);
    EXPECT_EQ(trades[0].quantity, 36);
    // check the resting order is still there with reduced quantity
    EXPECT_EQ(book.amount_at_front(false, 50.00), 64);

    Order buyMarket{2, 50.00, 6, true, 0, OrderType::Market};
    trades = book.match_order(buyMarket);

    ASSERT_EQ(trades.size(), 1);
    EXPECT_EQ(trades[0].quantity, 6);
    EXPECT_EQ(book.amount_at_front(false, 50.00), 58);
}

TEST(MatchingTest, MultiLevelWalk) {
    OrderBook book;
    // TODO: your Day 1 hand-worked example:
    // resting sells at 50.10 (qty 100) and 50.15 (qty 200),
    // incoming market buy for 250
    Order sell1{1, 50.10, 100, false, 0, OrderType::Limit};
    Order sell2{1, 50.15, 200, false, 0, OrderType::Limit};
    book.add_order(sell1);
    book.add_order(sell2);

    Order buyMarket1{1, 50.20, 250, true, 0, OrderType::Market};
    auto trades = book.match_order(buyMarket1);

    ASSERT_EQ(trades.size(), 2);
    EXPECT_DOUBLE_EQ(trades[0].price, 50.10);
    EXPECT_EQ(trades[0].quantity, 100);
    EXPECT_DOUBLE_EQ(trades[1].price, 50.15);
    EXPECT_EQ(trades[1].quantity, 150);
    // check 50 remains resting at 50.15:
    EXPECT_EQ(book.amount_at_front(false, 50.15), 50);
}

TEST(MatchingTest, TimePriorityMatchesFirst) {
    OrderBook book;
    // two resting orders at the SAME price, added in an order
    Order sell1{1, 50.00, 50, false, 0, OrderType::Limit};
    Order sell2{2, 50.00, 50, false, 0, OrderType::Limit};
    book.add_order(sell1);
    book.add_order(sell2);
    
    Order buyLimit{1, 51.00, 75, true, 0, OrderType::Limit};
    auto trades = book.match_order(buyLimit);

    ASSERT_EQ(trades.size(), 2);
    EXPECT_EQ(trades[0].sell_order_id, 0);
    EXPECT_EQ(trades[1].sell_order_id, 1);
}

TEST(MatchingTest, NonCrossingOrderRestsOnBook) {
    OrderBook book;
    // incoming MARKET buy that will not rest on the book but will still trade
    Order buyMarket{1, 49.00, 60, true, 0, OrderType::Market};
    auto trades = book.match_order(buyMarket);
    EXPECT_EQ(trades.size(), 0);
    EXPECT_EQ(book.amount_at_front(true, 49.00), -1);

    // resting sell at some price
    Order sell1{1, 50.00, 50, false, 0, OrderType::Limit};
    book.add_order(sell1);

    // incoming LIMIT buy priced too low to cross
    Order buyLimit{1, 49.00, 30, true, 0, OrderType::Limit};
    trades = book.match_order(buyLimit);
    EXPECT_EQ(trades.size(), 0);
    // check the incoming order now rests on the book
    EXPECT_EQ(book.amount_at_front(true, 49.00), 30);
}