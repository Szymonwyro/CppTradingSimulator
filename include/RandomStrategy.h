#pragma once

#include "Strategy.h"
#include "RandomEngine.h"
#include "Order.h"
#include "PriceStep.h"

class RandomStrategy : public Strategy {
private:
    RandomEngine& rng;

public:
    RandomStrategy(RandomEngine& rng_);

    std::vector<Order> generateOrders(
        int traderId,
        const std::vector<PriceStep>& history
    ) override;
};