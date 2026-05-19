#pragma once

#include "Strategy.h"
#include "Order.h"
#include "PriceStep.h"

class MomentumStrategy : public Strategy {
    private:
        double momentumFactor;
    
    public:
    MomentumStrategy(double momentumFactor);

    Order generateOrder(
        int traderId,
        const PriceStep& market
    ) override;
};