#pragma once

#include "Strategy.h"
#include "Order.h"
#include "PriceStep.h"

class ASMarketMakingStrategy : public Strategy {
    private:
        double gamma; // risk aversion parameter
        double orderSize; // fixed order size
        int horizon; // time horizon for the strategy
        double k; // market making parameter
        double inventory;

    public:
        ASMarketMakingStrategy(
            double gamma,
            double orderSize,
            int horizon,
            double k,
            double initialInventory = 0.0
        );

    std::vector<Order> generateOrders(
        int traderId,
        const std::vector<PriceStep>& history
    ) override;

    double getInventory() const { return inventory; }

    double onFill(double qty); // Update inventory and return PnL from the fill
};