#include <iostream>
#include <vector>
#include "RandomEngine.h"
#include "RandomStrategy.h"
#include "PriceStep.h"
#include "Order.h"
#include "NewsGenerator.h"
#include "PriceModel.h"


int main () {
    RandomEngine priceRng(42);
    RandomEngine strategyRng(123); //separate seed for trader rather than seed for price path
    NewsGenerator newsGen;

    PriceModel model(100.0, 0.01, 0.2, priceRng, &newsGen, 100); // initialPrice, drift, volatility, rng, newsGen, horizon
    RandomStrategy strategy(strategyRng);

    std::vector<PriceStep> history;
    double dt = 1.0 / 252.0; // daily steps

    for (int i = 0; i < 100; ++i) {
        model.step(dt);

        double newMid = model.getPrice();
        double prevMid = history.empty() ? newMid : history.back().mid;

        history.push_back({
            i,
            newMid,
            prevMid,
            newMid - 0.5,
            newMid + 0.5,
            1.0, // spread
            model.getLastDrift(),
            model.getLastVol(),
            model.getLastJump(),
            {}
        });

        std::vector<Order> orders = strategy.generateOrders(history);
    
            for (const auto& order : orders) {
                std::cout 
                        << "Day: " << i
                        << "Trader: RandomTrader" // << order.traderId
                        << " | " << (order.side == Side::BUY ? "BUY" : "SELL")
                        << " | qty: " << order.quantity
                        << " | Price: " << newMid
                        << " | limit price: " << order.price << "\n";
            }
    }

    return 0;

};
