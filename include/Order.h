#pragma once

#include <string>

enum class Side {
    BUY,
    SELL
};

struct Order {
    std::string traderId;
    Side side;
    double price;
    int quantity;
};