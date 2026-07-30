#include "RandomStrategy.h"
#include "PriceStep.h"
#include <random>

RandomStrategy::RandomStrategy(RandomEngine& engine) : rng(engine) {}

std::string RandomStrategy::getTypeTag() const {
    return "RandomTrader";

}

std::vector<Order> RandomStrategy::generateOrders(
    const std::vector<PriceStep>& history
) {
    const PriceStep& market = history.back();
    bool buy = rng.uniform() < 0.5;

    double offset = rng.normal() * 0.002;

    double limitPrice;

    if (buy) {
        limitPrice = market.bid * (1 - offset);
    } else {
        limitPrice = market.ask * (1 + offset);
    }
    int quantity = rng.randint(1, 10);

    return {{
            "",
            buy ? Side::BUY : Side::SELL,
            limitPrice,
            quantity
        }
    };
}
