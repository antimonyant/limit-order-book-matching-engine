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
    book.print_order_book();

    // assert first so that trades[0] is valid
    ASSERT_EQ(trades.size(), 1);
    EXPECT_EQ(trades[0].quantity, 100);
    EXPECT_DOUBLE_EQ(trades[0].price, 50.00);
    // check the resting side
    EXPECT_EQ(book.best_ask(), 67.91);

    // Match a MARKET buy that exactly matches its quantity
    Order buyMarket{2, 71.00, 42, true, 1, OrderType::Market};
    trades = book.match_order(buyMarket);
    book.print_order_book();
    ASSERT_EQ(trades.size(), 1);
    EXPECT_EQ(trades[0].quantity, 42);
    EXPECT_DOUBLE_EQ(trades[0].price, 67.91);
    EXPECT_EQ(book.best_ask(), -1.0);
}

TEST(MatchingTest, PartialFill) {
    OrderBook book;
    // TODO: resting sell with MORE quantity than the incoming order
    // auto trades = book.match_order(incoming);

    // TODO: ASSERT_EQ(trades.size(), 1);
    // TODO: EXPECT_EQ(trades[0].quantity, /* the smaller amount */);
    // TODO: check the resting order is still there with reduced quantity:
    //       EXPECT_EQ(book.quantity_at_front(/*is_buy=*/false, price), remaining);
}

TEST(MatchingTest, MultiLevelWalk) {
    OrderBook book;
    // TODO: your Day 1 hand-worked example:
    // resting sells at 50.10 (qty 100) and 50.15 (qty 200),
    // incoming market buy for 250
    // auto trades = book.match_order(incoming);

    // TODO: ASSERT_EQ(trades.size(), 2);
    // TODO: EXPECT_DOUBLE_EQ(trades[0].price, 50.10);
    // TODO: EXPECT_EQ(trades[0].quantity, 100);
    // TODO: EXPECT_DOUBLE_EQ(trades[1].price, 50.15);
    // TODO: EXPECT_EQ(trades[1].quantity, 150);
    // TODO: check 50 remains resting at 50.15:
    //       EXPECT_EQ(book.quantity_at_front(/*is_buy=*/false, 50.15), 50);
}

TEST(MatchingTest, TimePriorityMatchesFirst) {
    OrderBook book;
    // TODO: two resting orders at the SAME price, added in a specific order
    // TODO: submit an incoming order that only fills one of them
    // auto trades = book.match_order(incoming);

    // TODO: ASSERT_EQ(trades.size(), 1);
    // TODO: EXPECT_EQ(trades[0].sell_order_id, /* id of whichever added FIRST */);
}

TEST(MatchingTest, NonCrossingOrderRestsOnBook) {
    OrderBook book;
    // TODO: resting sell at some price
    // TODO: incoming LIMIT buy priced too low to cross
    // auto trades = book.match_order(incoming);

    // TODO: EXPECT_EQ(trades.size(), 0);
    // TODO: check the incoming order now rests on the book — e.g.
    //       EXPECT_EQ(book.quantity_at_front(/*is_buy=*/true, incoming_price), incoming_qty);
}