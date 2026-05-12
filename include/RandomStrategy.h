#pragma once

#include "strategy.h"
#include "RandomEngine.h"
#include "Order.h"

class RandomStrategy : public Strategy {
private:
    RandomEngine& rng;

public:
    RandomStrategy(RandomEngine& rng_);

    Order generateOrder(
        int traderId,
        const MarketState& market
    ) override;
};