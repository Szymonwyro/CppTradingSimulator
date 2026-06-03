#include <iostream>
#include <vector>
#include "RandomEngine.h"
#include "RandomStrategy.h"
#include "PriceStep.h"
#include "Order.h"
#include "NewsGenerator.h"
#include "PriceModel.h"


int main () {
    RandomEngine rng(42);
    NewsGenerator newsGen;

    PriceModel model(100.0, 0.01, 0.2, rng, &newsGen, 20); // initialPrice, drift, volatility, rng, newsGen, horizon
    RandomStrategy strategy(rng);

    std::vector<PriceStep> history;
    double dt = 1.0 / 252.0; // daily steps

    for (int i = 0; i < 20; ++i) {
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
    }

    return 0;

};
