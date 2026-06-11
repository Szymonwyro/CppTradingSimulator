#pragma once

#include "Strategy.h"
#include "Order.h"
#include "PriceStep.h"
#include <vector>

class MomentumStrategy : public Strategy {
    private:
        std::string traderId;
        double momentumFactor;
        int lookbackPeriod;
        double threshold;

    public:
        MomentumStrategy(
            std::string traderId,
            double momentumFactor,
            int lookbackPeriod,
            double threshold
        );

    std::vector<Order> generateOrders(
        const std::string traderId,
        const std::vector<PriceStep>& history
    ) override;
};