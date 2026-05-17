#pragma once

#include <vector>
#include <string>
#include "NewsEvent.h"


struct PriceStep {
    int day;
    double mid;
    double prevMid;
    double bid;
    double ask;

    double spread;
    double drift;
    double vol;
    double jump;
    std::vector<std::string> news;
};