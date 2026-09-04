//
// Created by 44785 on 02/08/2026.
//

#ifndef BETTINGARBITRAGEENGINE_ORDERBOOK_HPP
#define BETTINGARBITRAGEENGINE_ORDERBOOK_HPP

#include <mutex>
#include <unordered_map>

#include "event.hpp"

class OrderBook {
public:
    // Update the OrderBook snapshot
    void update(const OddsUpdate& update);

    // Gets the odds for a given event and outcome
    std::vector<OddsUpdate> getOddsFor(const std::string& event_id, const std::string& outcome) const;

    void removeOddsFor(const std::string& event_id, double odds, const std::string& outcome, const std::string& bookmaker);

private:
    // Event Id -> Outcome -> Bookmaker -> Odds
    std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<std::string, OddsUpdate>>> order_book_;
    mutable std::mutex mtx_order_book_;
};


#endif //BETTINGARBITRAGEENGINE_ORDERBOOK_HPP
