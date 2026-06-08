#pragma once

#include <vector>
#include <string>

struct AgentMetrics {
    double totalPnL;
    double sharpeRatio;
    double maxDrawdown;
    double inventoryRMSE;
    double fillRate;
    int totalFills;
    int totalOrdersSubmitted;
};

AgentMetrics computeMetrics(
    const std::vector<double>& pnlHistory,
    const std::vector<double>& inventoryHistory,
    int totalFills,
    int totalOrdersSubmitted
);