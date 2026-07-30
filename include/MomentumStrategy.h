#pragma once

#include "Strategy.h"
#include "Order.h"
#include "PriceStep.h"
#include <vector>

class MomentumStrategy : public Strategy {
    private:
        double momentumFactor;
        int lookbackPeriod;
        double threshold;

    public:
        MomentumStrategy(
            double momentumFactor,
            int lookbackPeriod,
            double threshold
        );

    std::vector<Order> generateOrders(
        const std::vector<PriceStep>& history
    ) override;

    std::string getTypeTag() const override;
};