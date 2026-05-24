#pragma once
#include "Order.h"
#include "PriceStep.h"
#include <vector>

class OrderBook;

class Strategy {
public:
    virtual ~Strategy() = default;

    virtual std::vector<Order> generateOrders(
        int traderId,
        const PriceStep& market
    ) = 0;
};
