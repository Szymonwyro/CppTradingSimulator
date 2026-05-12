#include <iostream>
#include "RandomEngine.h"
#include "RandomStrategy.h"
#include "MarketState.h"

int main() {
    RandomEngine rng(42);

    MarketState market{
        market.bestBid = 100.0,
        market.bestAsk = 101.0,
        market.midPrice = 100.5,
        market.volatility = 0.2
    };

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
