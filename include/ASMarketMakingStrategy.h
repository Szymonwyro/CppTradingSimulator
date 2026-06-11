#pragma once

#include "Strategy.h"
#include "Order.h"
#include "PriceStep.h"
#include <vector>
#include <cmath>

class ASMarketMakingStrategy : public Strategy {
    private:
        std::string traderId;
        double gamma;
        double orderSize;
        int horizon;
        double k;
        double inventory;

    public:
        ASMarketMakingStrategy(
            std::string traderId,
            double gamma,
            double orderSize,
            int horizon,
            double k,
            double initialInventory = 0.0
        );

        std::vector<Order> generateOrders(
            const std::string traderId,
            const std::vector<PriceStep>& history
        ) override;

        void onFill(double qty);

        double getInventory() const;
};