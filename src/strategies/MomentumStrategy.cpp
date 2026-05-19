#include "MomentumStrategy.h"
#include "PriceStep.h"

MomentumStrategy::MomentumStrategy(double momentumFactor, int lookbackPeriod, double threshold)
    : momentumFactor(momentumFactor), lookbackPeriod(lookbackPeriod), threshold(threshold) {}

Order MomentumStrategy::generateOrder(
    int traderId,
    const PriceStep& market
) {
    priceHistory.push_back(market.mid);

    if (priceHistory.size() < lookbackPeriod) {
        return Order{0, Side::BUY, 0.0, 0}; // No order if we do not have enough history
    }

    double oldPrice = priceHistory[priceHistory.size() - lookbackPeriod];

    double momentum = (market.mid - oldPrice) / oldPrice;

    if (priceHistory.size() > lookbackPeriod) {
        priceHistory.pop_front(); // Keep only the last 'lookbackPeriod' prices
    }

    if (std::abs(momentum) < threshold) {
        return Order{0, Side::BUY, 0.0, 0}; // No order if momentum is below threshold
    }

    bool buy = momentum > 0;
    double offset = momentum * momentumFactor;
    double limitPrice;

    if (buy) {
        limitPrice = market.bid * (1 + offset);
    } else {
        limitPrice = market.ask * (1 + offset);
    }
    int quantity = 1; // Fixed quantity for simplicity

    return {
        traderId,
        buy ? Side::BUY : Side::SELL,
        limitPrice,
        quantity
    };
}
