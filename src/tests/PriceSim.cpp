#include <iostream>
#include "RandomEngine.h"
#include "PriceModel.h"
#include "NewsGenerator.h"
#include "NewsEvent.h"
#include "PriceStep.h"
#include "PriceStep.h"
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

int main() {
    RandomEngine rng(42);
    NewsGenerator newsGen;

    int horizon = 2016; // 8 years of trading days

    double initialPrice = 100.0;
    double drift = 0.05;
    double volatility = 0.2;

    PriceModel model(initialPrice, drift, volatility, rng, &newsGen, horizon);

    double dt = 1.0/252.0;

    std::vector<PriceStep> history;

    for (int i = 0; i < horizon; ++i) {
        model.step(dt);

        // Compute bid, ask, and spread using a simple spread model
        double mid = model.getPrice();
        double vol = model.getLastVol();

        double base_spread = 5.0; // big spread just for visibility in plot
        double k = 5.0; // sensitivity to volatility
        
        double spread = base_spread + k * vol;
        double bid = mid - spread / 2.0;
        double ask = mid + spread / 2.0;

        double prevMid = (i > 0) ? history.back().mid : mid;
    

        //Collect news for the day
        std::vector<std::string> todaysNews;
        for (const auto& news : model.getNewsEvents()) {
            if (news.day == i) {
                todaysNews.push_back(news.headline);
            }
        }

        std::string newsStr = "No news";
        if (!todaysNews.empty()) {
            newsStr = "";
            for (const auto& headline : todaysNews) {
                newsStr += headline + " | ";
            }
            newsStr = newsStr.substr(0, newsStr.size() - 3); // Remove trailing " | "
        }

        history.push_back({ 
            i,
            mid,
            prevMid,
            bid,
            ask,
            spread,
            model.getLastDrift(),
            vol,
            model.getLastJump(),
            std::vector<std::string>(todaysNews)});

    }

    // Output results to CSV

    std::ofstream outFile("price_simulation.csv");
    outFile << "Day,Mid,Bid,Ask,Spread,Drift,Volatility,Jump,News\n";

    for (const auto& step : history) {
        outFile << step.day << ","
                << step.mid << ","
                << step.prevMid << ","
                << step.bid << ","
                << step.ask << ","
                << step.spread << ","
                << step.drift << ","
                << step.vol << ","
                << step.jump << ",";

        if (step.news.empty()) {
            outFile << "No news";
        } else {
            outFile << "\"";
            for (size_t j = 0; j < step.news.size(); ++j) {
                if (j > 0) outFile << " | ";
                outFile << step.news[j];
            }
            outFile << "\"";
        }
        outFile << "\n";
    }

    outFile.close();

    std::cout << "Simulation complete. Results saved to price_simulation.csv" << std::endl;

    return 0;

}
