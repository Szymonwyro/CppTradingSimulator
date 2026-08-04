#pragma once
#include "Order.h"
#include "RandomEngine.h"

struct FillModelParams {
    double baseRate       = 0.9;   // fill probability right at the touch (distance = 0)
    double distanceDecay  = 5.0;   // higher = fill probability drops off faster as price worsens
    double sizePenalty    = 0.01;  // probability lost per unit of quantity quoted
};

class FillModel {
public:
    explicit FillModel(FillModelParams params = {});

    // Probability in [0,1] that this order fills this tick, given the current mid.
    double computeFillProbability(const Order& order, double mid) const;

    // Rolls the dice — true if it fills.
    bool rollFill(const Order& order, double mid, RandomEngine& rng) const;

private:
    FillModelParams params_;
};