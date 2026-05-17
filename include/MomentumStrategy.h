#pragma once

#include "strategy.h"
#include "Order.h"

class MomentumStrategy : public Strategy {
    private:
        double momentumFactor;
    
    public:
    MomentumStrategy(double momentumFactor);

    Order generateOrder(
        int traderId,
        const MarketState& market
    ) override;
};