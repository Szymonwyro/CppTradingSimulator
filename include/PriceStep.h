#pragma once

struct PriceStep {
    int day;
    double mid;
    double lastPrice;
    double bid;
    double ask;

    double spread;
    double drift;
    double vol;
    double jump;
    std::vector<std::string> news;
}