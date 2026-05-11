#pragma once
#include "RandomEngine.h"
#include "NewsEvent.h"
#include "NewsGenerator.h"
#include <vector>

class PriceModel {

    public:
        PriceModel(double initial_price, double drift, double volatility, RandomEngine& rng, NewsGenerator* newsGen = nullptr, int horizon = 0);

        void step(double dt);       //advance price by dt
        double getPrice() const;    //return current price
        std::vector<double> simulatePath(int steps, double dt);
        const std::vector<NewsEvent>& getNewsEvents() const;

        double getLastDrift() const { return lastDrift; }
        double getLastVol() const { return lastVol; }
        double getLastJump() const { return lastJump; }

    private:

        int currentDay = 0;
        double price;
        double mu;
        double sigma;
        RandomEngine& rng;

        //Mean-reversion parameters for baseline volatility
        double sigma_baseline = 0.012;   //initial baseline volatility
        double theta = 0.05;            //speed of mean reversion  
        double eta = 0.005;             //volatility of volatility
        double sigma0 = 0.012;           //long-term mean of baseline volatility

        NewsGenerator* newsGen;
        std::vector<NewsEvent> newsEvents;

        double lastDrift = 0.0;
        double lastVol = 0.0;
        double lastJump = 0.0;
        
        double applyNews(double basePrice);

};

