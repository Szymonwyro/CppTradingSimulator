#pragma once
#include "Order.h"
#include "MarketState.h"

class OrderBook;

class Strategy {
public:
    virtual ~Strategy() = default;

    virtual Order generateOrder(
        int traderId,
        const MarketState& market
    ) = 0;
};
