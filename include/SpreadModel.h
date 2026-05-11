#pragma once

#include "PriceModel.h"

class SpreadModel {
public:
    SpreadModel(double base_spread, double k);

    double computeSpread(double price, double volatility);

private:
    double base_spread;
    double k; // sensitivity to volatility
};