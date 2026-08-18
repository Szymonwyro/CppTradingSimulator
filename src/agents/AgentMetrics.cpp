#include "AgentMetrics.h"
#include <numeric>
#include <cmath>
#include <algorithm>
#include <stdexcept>

AgentMetrics computeMetrics(

    const std::vector<double>& pnlHistory,
    const std::vector<double>& inventoryHistory,
    int totalFills,
    int totalOrdersSubmitted
) {
    AgentMetrics m{};

    if (pnlHistory.empty()) return m;

    // Total PnL
    m.totalPnL = pnlHistory.back();
    m.totalFills = totalFills;
    m.totalOrdersSubmitted = totalOrdersSubmitted;

    // Fill rate
    m.fillRate = totalOrdersSubmitted > 0
        ? static_cast<double>(totalFills) / totalOrdersSubmitted
        : 0.0;

    // Daily returns
    std::vector<double> returns;
    returns.reserve(pnlHistory.size() - 1);
    for (size_t i = 1; i < pnlHistory.size(); ++i) {
        returns.push_back(pnlHistory[i] - pnlHistory[i - 1]);
    }

    // Sharpe ratio (annualised, assuming daily returns, 252 trading days)
    if (!returns.empty()) {
        double mean = std::accumulate(returns.begin(), returns.end(), 0.0) / returns.size();
        double variance = 0.0;
        for (double r : returns) variance += (r - mean) * (r - mean);
        variance /= returns.size();
        double stddev = std::sqrt(variance);
        m.sharpeRatio = stddev > 0.0 ? (mean / stddev) * std::sqrt(252.0) : 0.0;
    }

    // Max drawdown
    double peak = pnlHistory[0];
    double maxDD = 0.0;
    for (double pnl : pnlHistory) {
        peak = std::max(peak, pnl);
        maxDD = std::max(maxDD, peak - pnl);
    }
    m.maxDrawdown = maxDD;

    // Inventory RMSE — how far the trader strayed from zero inventory
    double sumSq = 0.0;
    for (double inv : inventoryHistory) sumSq += inv * inv;
    m.inventoryRMSE = std::sqrt(sumSq / inventoryHistory.size());

    return m;
}