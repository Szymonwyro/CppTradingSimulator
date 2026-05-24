#include <iostream>
#include "PriceStep.h"
#include "MomentumStrategy.h"

int main () {
    PriceStep market{
        market.day = 0,
        market.bid = 100.0,
        market.ask = 101.0,
        market.mid = 100.5,
        market.prevMid = 100.0,
        market.vol = 0.2,
        market.drift = 0.01,
        market.jump = 0.0
    };

    MomentumStrategy strategy(2.0, 3, 0.01); // Momentum factor of 2, lookback period of 3, threshold of 0.01

    for (int i = 0; i < 20; ++i) {
        std::vector<Order> orders = strategy.generateOrders(1, market);

        for (const auto& order : orders) {
            std::cout << "Trader " << order.traderId 
                      << " places a " << (order.side == Side::BUY ? "BUY" : "SELL") 
                      << " order for " << order.quantity 
                      << " shares at price " << order.price << std::endl;
        }

        // Simulate price change for next iteration
        market.prevMid = market.mid;
        market.mid += (market.drift + market.vol * ((rand() / double(RAND_MAX)) - 0.5)) * market.mid;
        market.bid = market.mid - 0.5;
        market.ask = market.mid + 0.5;
    }

    return 0;
}
