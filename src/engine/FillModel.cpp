#include "FillModel.h"
#include <cmath>
#include <algorithm>

FillModel::FillModel(FillModelParams params) : params_(params) {}

double FillModel::computeFillProbability(const Order& order, double mid) const {
    double distance = std::abs(order.price - mid) / mid;   // scale-free distance
    double distanceFactor = std::exp(-params_.distanceDecay * distance);

    double sizeFactor = std::max(0.0, 1.0 - params_.sizePenalty * order.quantity);

    double probability = params_.baseRate * distanceFactor * sizeFactor;
    return std::clamp(probability, 0.0, 1.0);
}

bool FillModel::rollFill(const Order& order, double mid, RandomEngine& rng) const {
    return rng.uniform() < computeFillProbability(order, mid);
}