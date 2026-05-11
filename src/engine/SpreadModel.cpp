#include "SpreadModel.h"

SpreadModel::SpreadModel(double base_spread, double k) : base_spread(base_spread), k(k) {}

double SpreadModel::computeSpread(double price, double volatility) {
    return base_spread + k * volatility;
}