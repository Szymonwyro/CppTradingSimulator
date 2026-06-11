#include "ASMarketMakingStrategy.h"
#include "PriceStep.h"
#include "NewsGenerator.h"
#include "PriceModel.h"
#include "RandomEngine.h"
#include <iostream>
#include <vector>

int main() {
    RandomEngine rng(42);
    NewsGenerator newsGen;

    PriceModel model(100.0, 0.01, 0.2, rng, &newsGen, 20); // initialPrice, drift, volatility, rng, newsGen, horizon
    ASMarketMakingStrategy strategy("ASTrader", 0.1, 5, 20, 5.0, 0.0); // traderId, gamma, orderSize, horizon, k, inventory

    std::vector<PriceStep> history;
    double dt = 1.0 / 252.0; // daily steps

    for(int i=0, inventory=0; i<20; ++i) {
        model.step(dt);

        double newMid = model.getPrice();
        double prevMid = history.empty() ? newMid : history.back().mid;

        history.push_back({
            i,
            newMid,
            prevMid,
            newMid - 0.5,
            newMid + 0.5,
            1.0,
            model.getLastDrift(),
            model.getLastVol(),
            model.getLastJump(),
            {}
        });

        std::vector<Order> orders = strategy.generateOrders("ASTrader", history);

        if (orders.size() == 2) {
            double bid = orders[0].price;
            double ask = orders[1].price;
            double reservationPrice = (bid + ask) / 2.0;
            double spread = ask - bid;

            for (const auto& order : orders) {
                std::cout
                      << " | Trader: " << order.traderId
                      << " | " << (order.side == Side::BUY ? "BUY" : "SELL")
                      << " | qty: " << order.quantity
                      << " | Market Price: " << newMid << "\n"
                      << " | Reservation Price: " << (order.side == Side::BUY ? newMid - 0.5 : newMid + 0.5) << "\n"
                      << " | Spread: " << spread << "\n"
                      << " | Inventory: " << strategy.getInventory() << "\n";
            }   
        }
    }
    return 0;
}