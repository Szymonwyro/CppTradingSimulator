#include "MomentumStrategy.h"

MomentumStrategy::MomentumStrategy(
    std::string traderId,
    double momentumFactor,
    int lookbackPeriod,
    double threshold)
    : traderId(traderId), momentumFactor(momentumFactor), lookbackPeriod(lookbackPeriod), threshold(threshold) {}

std::vector<Order> MomentumStrategy::generateOrders(
    const std::string traderId,
    const std::vector<PriceStep>& history
) {
    if ((int)history.size() < lookbackPeriod) {
        return {};
    }

    double oldPrice = history[history.size() - lookbackPeriod].mid;
    double currentPrice = history.back().mid;
    double momentum = (currentPrice - oldPrice) / oldPrice;

    if (std::abs(momentum) < threshold) {
        return {};
    }

    bool buy = momentum > 0;
    double offset = momentum * momentumFactor;
    double limitPrice = buy
        ? history.back().bid * (1 + offset)
        : history.back().ask * (1 + offset);

    return {{
        traderId,
        buy ? Side::BUY : Side::SELL,
        limitPrice,
        1 // fixed quantity for simplicity
    }};
}
