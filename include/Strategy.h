#pragma once
#include "Order.h"
#include "PriceStep.h"

class OrderBook;

class Strategy {
public:
    virtual ~Strategy() = default;

    virtual Order generateOrder(
        int traderId,
        const PriceStep& market
    ) = 0;
};
