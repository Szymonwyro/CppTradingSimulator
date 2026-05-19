#include <iostream>
#include "RandomEngine.h"
#include "RandomStrategy.h"
#include "PriceStep.h"

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

    RandomEngine rng(42);
    RandomStrategy strategy(rng);
    
    for (int i = 0; i < 20; ++i) {

        Order order = strategy.generateOrder(1, market);

        std::cout << "Trader " << order.traderId 
                  << " places a " << (order.side == Side::BUY ? "BUY" : "SELL") 
                  << " order for " << order.quantity 
                  << " shares at price " << order.price << std::endl;
    }

    return 0;

};
