#include"Trader.h"
#include <stdexcept>

Trader::Trader(int id, std::string name, std::unique_ptr<Strategy> strategy, double startingCash)
    : id(id),
      name(std::move(name)),
      strategy(std::move(strategy)),
      cash(startingCash),
      startingCash(startingCash),
      inventory(0.0),
      ordersSubmitted(0)
{
    if (startingCash <= 0) {
        throw std::invalid_argument("Starting cash must be positive");
    }

    traderId = this->strategy->getTypeTag() + "_" + std::to_string(id);
}

std::vector<Order> Trader::generateOrders(const std::vector<PriceStep>& history) {
    auto orders = strategy->generateOrders(history);
    for (auto& order : orders) {
        order.traderId = traderId;
    }
    return orders;
}

void Trader::onFill(double price, double qty, Side side, int day) {
    if (side == Side::BUY) {
        inventory += qty;
        cash -= price * qty;
    } else {
        inventory -= qty;
        cash += price * qty;
    }

    // Fix: this never told the strategy about the fill, so a strategy that
    // tracks its own inventory (e.g. ASMarketMakingStrategy, for its
    // reservation-price skew) never actually saw it update.
    strategy->onFill(side == Side::BUY ? qty : -qty);

    fillHistory.push_back({ day, price, qty, side });
}

void Trader::recordState(double currentMid, int day) {
    pnlHistory.push_back(getTotalPnL(currentMid));
    inventoryHistory.push_back(inventory);
}

void Trader::recordOrdersSubmitted(int n) {
    ordersSubmitted += n;
}

int Trader::getOrdersSubmitted() const { return ordersSubmitted; }

double Trader::getUnrealisedPnL(double currentMid) const {
    return inventory * currentMid;
}

double Trader::getTotalPnL(double currentMid) const {
    return (cash - startingCash) + getUnrealisedPnL(currentMid);
}

int Trader::getId() const { return id; }
std::string Trader::getName() const { return name; }
std::string Trader::getTraderId() const { return traderId; }
double Trader::getInventory() const { return inventory; }
double Trader::getCash() const { return cash; }

const std::vector<double>& Trader::getPnLHistory() const { return pnlHistory; }
const std::vector<double>& Trader::getInventoryHistory() const { return inventoryHistory; }
const std::vector<Fill>& Trader::getFillHistory() const { return fillHistory; }