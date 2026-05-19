#pragma once

#include "Strategy.h"
#include "Order.h"
#include "PriceStep.h"

#include <deque>

class MomentumStrategy : public Strategy {
    private:
        double momentumFactor;
        int lookbackPeriod;
        double threshold;
        std::deque<double> priceHistory;

    public:
        MomentumStrategy(
            double momentumFactor,
            int lookbackPeriod,
            double threshold
        );

    Order generateOrder(
        int traderId,
        const PriceStep& market
    ) override;
};