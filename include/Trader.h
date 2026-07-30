#pragma once

#include "Strategy.h"
#include "Order.h"
#include "PriceStep.h"
#include <vector>
#include <memory>
#include <string>

struct Fill {
    int day;
    double price;
    double qty;
    Side side;
};

class Trader {
    private:
        int id;
        std::string name;
        std::string traderId;
        std::unique_ptr<Strategy> strategy;

        double cash;
        double startingCash;
        double inventory;

        std::vector<Fill> fillHistory;
        std::vector<double> pnlHistory;
        std::vector<double> inventoryHistory;

        int ordersSubmitted;

    public:
        Trader(int id, std::string name, std::unique_ptr<Strategy> strategy, double startingCash);

        std::vector<Order> generateOrders(const std::vector<PriceStep>& history);

        void onFill(double price, double qty, Side side, int day);

        void recordState(double currentMid, int day);

        int getId() const;
        std::string getName() const;
        std::string getTraderId() const;
        double getInventory() const;
        double getCash() const;
        double getUnrealisedPnL(double currentMid) const;
        double getTotalPnL(double currentMid) const;

        const std::vector<double>& getPnLHistory() const;
        const std::vector<double>& getInventoryHistory() const;
        const std::vector<Fill>& getFillHistory() const;

        void recordOrdersSubmitted(int n);
        int getOrdersSubmitted() const;
};