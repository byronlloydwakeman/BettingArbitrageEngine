//
// Created by 44785 on 31/07/2026.
//
// Data model that is produced by the bookmaker
//

#ifndef BETTINGARBITRAGEENGINE_EVENT_HPP
#define BETTINGARBITRAGEENGINE_EVENT_HPP
#include <chrono>
#include <string>


struct OddsUpdate {
    std::string event_id;
    std::string outcome;
    std::string bookmaker_id;
    double odds;
    std::chrono::system_clock::time_point timestamp;

    OddsUpdate(const std::string &event_id, const std::string &outcome, const std::string &bookmaker_id, double odds,
        const std::chrono::system_clock::time_point &timestamp)
        : event_id(event_id),
          outcome(outcome),
          bookmaker_id(bookmaker_id),
          odds(odds),
          timestamp(timestamp) {
    }
};

inline std::ostream& operator<<(std::ostream& os, const OddsUpdate& u) {
    return os << "Event: " << u.event_id
              << ", Outcome: " << u.outcome
              << ", Bookmaker: " << u.bookmaker_id
              << ", Odds: " << u.odds;
}

struct ArbWindow {
    std::string event_id;
    double implied_prob_sum;
    std::chrono::system_clock::time_point opened_at;
};




#endif //BETTINGARBITRAGEENGINE_EVENT_HPP
