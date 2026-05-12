#pragma once

enum class Side {
    BUY,
    SELL
};

struct Order {
    int traderId;
    Side side;
    double price;
    int quantity;
};