//
// Created by 44785 on 25/08/2026.
//

#ifndef BETTINGARBITRAGEENGINE_COMPUTEQUEUE_HPP
#define BETTINGARBITRAGEENGINE_COMPUTEQUEUE_HPP

class OrderBook;

struct ArbCheckRequest {
    std::string event_id;
};

// Single arbitragable action, to be used with other ArbResponses
struct ArbResponse {
    std::string event_id;
    std::string bookmaker_id;
    std::string outcome;
    double odds;

    ArbResponse() {
        event_id = "";
        bookmaker_id = "";
        outcome = "";
        odds = 0.0;
    }

    ArbResponse(const std::string &event_id, const std::string &bookmaker_id, const std::string &outcome, double odds)
        : event_id(event_id),
          bookmaker_id(bookmaker_id),
          outcome(outcome),
          odds(odds) {
    }

};


inline std::ostream& operator<<(std::ostream& os, const ArbResponse& a)
{
    os << "[Event: "      << a.event_id      << "] "
       << "[Outcome: "    << a.outcome       << "] "
       << "[Bookmaker: "  << a.bookmaker_id  << "] "
       << "[Odds: "       << a.odds          << "]";
    return os;
}

class IComputeQueue {
public:
    // Push an event id that has changed to the queue to compute
    void push(const ArbCheckRequest& arb_check_request);
    // Start new process
    void startConsuming(OrderBook& order_book);
    // Find arb opportunities
    std::vector<ArbResponse>  findArbitrage(const std::vector<OddsUpdate>& odds);
    void stop();
};

#endif //BETTINGARBITRAGEENGINE_COMPUTEQUEUE_HPP
