//
// Created by 44785 on 22/08/2026.
//

#ifndef BETTINGARBITRAGEENGINE_COMPUTEQUEUE_HPP
#define BETTINGARBITRAGEENGINE_COMPUTEQUEUE_HPP
#include <atomic>
#include <condition_variable>
#include <queue>
#include <string>
#include <thread>
#include <vector>

#include "event.hpp"
#include "orderBook.hpp"


struct ArbCheckRequest {
    std::string event_id;
};

struct ArbResponse {
    std::string event_id; // Pair??
    std::string bookmaker_id;
};

class ComputeQueue {
public:
    ComputeQueue();
    ~ComputeQueue();

    // Push an event id that has changed to the queue to compute
    void push(const ArbCheckRequest& arb_check_request);
    // Start new process
    void startConsuming(const OrderBook& order_book);
    // Find arb opportunities
    std::optional<ArbResponse> findArbitrage(const std::vector<OddsUpdate>& odds);
    void stop();

private:
    void run(const OrderBook& order_book);
    std::queue<ArbCheckRequest> queue_;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::atomic<bool> is_running_{false};
    std::thread consumer_thread_;
};




#endif //BETTINGARBITRAGEENGINE_COMPUTEQUEUE_HPP
