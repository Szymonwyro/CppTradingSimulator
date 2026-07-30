#pragma once

#include "Strategy.h"
#include "Order.h"
#include "PriceStep.h"
#include <vector>
#include <cmath>

class ASMarketMakingStrategy : public Strategy {
    private:
        double gamma;
        double orderSize;
        int horizon;
        double k;
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
            const std::vector<PriceStep>& history
        ) override;

        void onFill(double qty);
        std::string getTypeTag() const override;

        double getInventory() const;
};