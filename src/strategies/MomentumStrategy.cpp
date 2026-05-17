#include "MomentumStrategy.h"
#include "PriceStep.h"

MomentumStrategy::MomentumStrategy(double momentumFactor) : momentumFactor(momentumFactor) {}

Order MomentumStrategy::generateOrder(
    int traderId,
    const PriceStep& market
) {
    double priceChange = market.mid - market.prevMid;

    bool buy = priceChange > 0;

    double offset = momentumFactor * priceChange / market.prevMid;

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
