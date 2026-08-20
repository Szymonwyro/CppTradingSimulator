#include "PriceModel.h"
#include "NewsGenerator.h"
#include "NewsEvent.h"
#include <cmath> // for std::exp
#include <vector>

PriceModel::PriceModel(double initial_price, double drift, double volatility, RandomEngine& rng_, NewsGenerator* newsGen_, int horizon)
    : currentDay(0), price(initial_price), mu(drift), sigma(volatility), rng(rng_),
      sigma_baseline(volatility), sigma0(volatility),
      newsGen(newsGen_)
{
    if (newsGen) {
        newsEvents = newsGen->generateNews(horizon, rng);
    }
}

void PriceModel::step(double dt)
{
    //Reset effects from previous steps
    double sumJumps = 0.0;
    double driftImpact = 0.0;
    double volImpact = 0.0;

    lastJump = 0.0;

    // Calculate jump, drift, and volatility impacts from news events currently active

    for (const auto& news : newsEvents) {

        // Jump applies to day of event only
        if (news.day == currentDay) {
            sumJumps += news.jump;
        }
        
        // Drift and volatility impacts apply for duration of event
        if (news.day <= currentDay && news.day + news.duration > currentDay) {
            driftImpact += news.driftImpact;
            volImpact += news.volImpact;
        }
    }

    // Apply jump impacts first

    if (sumJumps != 0.0) {
        price *= std::exp(sumJumps);
        lastJump = sumJumps;
    }

    // Update baseline volatility (mean-reverting)
    double epsilonVol = rng.normal();
    sigma_baseline = theta * (sigma0 - sigma_baseline) * dt + sigma_baseline + eta * epsilonVol;

    // Ensure baseline volatility stays positive
    sigma_baseline = std::max(sigma_baseline, 0.0001);

    // Effective parameters
    double mu_eff = mu + driftImpact;
    // Multiplicative effect on volatility: baseline + news impact
    double sigma_eff = sigma_baseline * (1.0 + 3 * volImpact);
    // Cap effective volatility to prevent extreme values
    sigma_eff = std::min(sigma_eff, 1.0);

    // GBM Shock
    double epsilonPrice = rng.normal();
    double driftTerm = (mu_eff - 0.5 * sigma_eff * sigma_eff) * dt;
    double diffusionTerm = sigma_eff * std::sqrt(dt) * epsilonPrice;

    lastDrift = driftTerm;
    lastVol = diffusionTerm;

    price *= std::exp(driftTerm + diffusionTerm);

    currentDay++;

}

double PriceModel::getPrice() const
{
    return price;
}

std::vector<double> PriceModel::simulatePath(int steps, double dt)
{
    std::vector<double> path;
    path.push_back(price); //initial price

    for (int i = 0; i < steps; ++i)
    {
        step(dt);
        path.push_back(price);
    }

    return path;
    
}

const std::vector<NewsEvent>& PriceModel::getNewsEvents() const
{
    return newsEvents;
}
