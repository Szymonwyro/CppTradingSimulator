#include "NewsGenerator.h"
#include "NewsEvent.h"
#include "PriceModel.h"
#include "RandomEngine.h"
#include <random>

std::vector<NewsEvent> NewsGenerator::generateNews(int horizon, RandomEngine& rng)
{
    std::vector<NewsEvent> events;

    //Earnings Calendar

    NewsEvent e1;
    e1.day = 63;
    e1.headline = "Firm reports quarterly earnings";
    e1.jump = 0.05;
    e1.driftImpact = 0.02;
    e1.volImpact = 0.15;
    e1.duration = 1;
    events.push_back(e1);

    NewsEvent e2;
    e2.day = 126;
    e2.headline = "Firm reports quarterly earnings";
    e2.jump = 0.05;
    e2.driftImpact = 0.02;
    e2.volImpact = 0.15;
    e2.duration = 1;
    events.push_back(e2);

    NewsEvent e3;
    e3.day = 189;
    e3.headline = "Firm reports quarterly earnings";
    e3.jump = 0.05;
    e3.driftImpact = 0.02;
    e3.volImpact = 0.15;
    e3.duration = 1;
    events.push_back(e3);

    NewsEvent e4;
    e4.day = 252;
    e4.headline = "Firm reports quarterly earnings";
    e4.jump = 0.05;
    e4.driftImpact = 0.02;
    e4.volImpact = 0.15;
    e4.duration = 1;
    events.push_back(e4);

    //Random Events (Macro and Rumor)

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> dayDist(1,horizon);
    std::uniform_int_distribution<> typeDist(0,1);
    std::uniform_int_distribution<> signDist(0,1);


    double dailyEventProb = 0.05;

    for (int day = 1; day <= horizon; ++day) {
        if (rng.uniform() < dailyEventProb ) {
            NewsEvent event;
            event.day = day;
            bool positive = rng.randint(0,1);

            if (rng.randint(0, 1) == 0) {
                // Macro Shock
                event.headline = "Global macro uncertainty rises";
                event.jump = positive ? 0.05 : -0.05;
                event.driftImpact = positive ? 0.01 : -0.01;
                event.volImpact = 0.3;
                event.duration = 3;
                
            } else {
                // Rumor
                event.headline = "Rumor: firm exploring strategic options";
                event.jump = (positive ? 0.03 : -0.03);
                event.driftImpact = positive ? 0.005 : -0.005;
                event.volImpact = 0.2;
                event.duration = 2;
            }

        events.push_back(event);
    }

    return events;

}