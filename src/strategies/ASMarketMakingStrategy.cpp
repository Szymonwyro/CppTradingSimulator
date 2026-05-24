#include "ASMarketMakingStrategy.h"
#include "PriceStep.h"
#include <cmath>

ASMarketMakingStrategy::ASMarketMakingStrategy(
    double gamma,
    double orderSize,
    int horizon,
    double k,
    double initialInventory
) : gamma(gamma), orderSize(orderSize), horizon(horizon), k(k), inventory(initialInventory) {}

// void ASMarketMakingStrategy::onFill(double qty) {
//    inventory += qty;
//}

std::vector<Order> ASMarketMakingStrategy::generateOrders(
    int traderId,
    const PriceStep& market
) {
    double t = market.day / static_cast<double>(horizon); // Normalize time to [0,1]
    double timeRemaining = (1.0 - t);

    double sigma = market.vol;
    double reservationPrice = market.mid - inventory * gamma * sigma * sigma * timeRemaining;
    double halfSpread = 1.0 / gamma * std::log(1 + gamma / k) + 0.5 * gamma * sigma * sigma * timeRemaining;

    double bidPrice = reservationPrice - halfSpread;
    double askPrice = reservationPrice + halfSpread;

    return {
        { traderId, Side::BUY,  bidPrice, static_cast<int>(orderSize) },
        { traderId, Side::SELL, askPrice, static_cast<int>(orderSize) }
    };
}



