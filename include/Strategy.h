#pragma once
#include "Order.h"
#include "PriceStep.h"
#include <vector>
#include <string>

class OrderBook;

class Strategy {
public:
    virtual ~Strategy() = default;

    virtual std::vector<Order> generateOrders(
        const std::vector<PriceStep>& history
    ) = 0;
    virtual std::string getTypeTag() const = 0;
    virtual void onFill(double qty) {}
};
