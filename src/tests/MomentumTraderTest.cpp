#include <iostream>
#include <vector>
#include <cmath>
#include "PriceStep.h"
#include "NewsGenerator.h"
#include "PriceModel.h"
#include "MomentumStrategy.h"
#include "RandomEngine.h"
#include "Order.h"

int main() {
    RandomEngine rng(42);
    NewsGenerator newsGen;

    PriceModel model(100.0, 0.01, 0.2, rng, &newsGen, 20); // initialPrice, drift, volatility, rng, newsGen, horizon

    MomentumStrategy strategy(2.0, 3, 0.01); // momentumFactor, lookbackPeriod, threshold

    std::vector<PriceStep> history;
    double dt = 1.0 / 252.0; // daily steps

    for (int i = 0; i < 20; ++i) {
        model.step(dt);

        double newMid = model.getPrice();
        double prevMid = (i > 0) ? history.back().mid : newMid;

        history.push_back({
            i,
            newMid,
            prevMid,
            newMid - 0.5,
            newMid + 0.5,
            1.0,
            0.01,
            0.2,
            0.0,
            {}
        });

        std::vector<Order> orders = strategy.generateOrders(1, history);

        if (orders.empty()) {
            std::cout << "Day " << i << ": no signal\n";
        }

        for (const auto& order : orders) {
            std::cout << "Day " << i
                      << " | Trader " << order.traderId
                      << " | " << (order.side == Side::BUY ? "BUY" : "SELL")
                      << " | qty: " << order.quantity
                      << " | price: " << order.price << "\n";
        }
    }

    return 0;
}
