#include <iostream>
#include <vector>
#include <cmath>
#include "PriceStep.h"
#include "MomentumStrategy.h"
#include "RandomEngine.h"
#include "Order.h"

int main() {
    RandomEngine rng(42);

    std::vector<PriceStep> history;
    history.push_back({
        0,      // day
        100.5,  // mid
        100.0,  // prevMid
        100.0,  // bid
        101.0,  // ask
        1.0,    // spread
        0.01,   // drift
        0.2,    // vol
        0.0,    // jump
        {}      // news
    });

    MomentumStrategy strategy(2.0, 3, 0.01);

    for (int i = 1; i < 20; ++i) {
        double prevMid = history.back().mid;
        double newMid = prevMid * std::exp(
            (0.01 - 0.5 * 0.2 * 0.2) * (1.0/252.0)
            + 0.2 * std::sqrt(1.0/252.0) * rng.normal()
        );

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
