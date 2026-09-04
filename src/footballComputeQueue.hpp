//
// Created by 44785 on 22/08/2026.
//

#ifndef BETTINGARBITRAGEENGINE_FOOTBALLCOMPUTEQUEUE_HPP
#define BETTINGARBITRAGEENGINE_FOOTBALLCOMPUTEQUEUE_HPP
#include <atomic>
#include <condition_variable>
#include <queue>

#include <thread>
#include <vector>

#include "orderBook.hpp"
#include "computeQueue.hpp"
#include "metricsRecorder.hpp"


namespace prometheus {
    class Registry;
}

class FootballComputeQueue : public IComputeQueue {
public:
    explicit FootballComputeQueue(std::shared_ptr<IMetricsRecorder> metrics_recorder);
    ~FootballComputeQueue();

    // Push an event id that has changed to the queue to compute
    void push(const ArbCheckRequest& arb_check_request);
    // Start new process
    void startConsuming(OrderBook& order_book);
    // Find arb opportunities
    std::vector<ArbResponse>  findArbitrage(const std::vector<OddsUpdate>& odds);
    void stop();

private:
    void run(OrderBook& order_book);
    std::queue<ArbCheckRequest> queue_;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::atomic<bool> is_running_{false};
    std::thread consumer_thread_;
    std::shared_ptr<IMetricsRecorder> metrics_recorder_;
};




#endif //BETTINGARBITRAGEENGINE_FOOTBALLCOMPUTEQUEUE_HPP
