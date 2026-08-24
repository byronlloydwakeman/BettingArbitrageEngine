//
// Created by 44785 on 22/08/2026.
//

#include "computeQueue.hpp"

#include <iostream>

ComputeQueue::ComputeQueue() = default;

ComputeQueue::~ComputeQueue() {
    stop();
}

void ComputeQueue::push(const ArbCheckRequest& arb_check_request) {
    std::unique_lock lock(mutex_);
    queue_.push(arb_check_request);
    cv_.notify_all();
}

void ComputeQueue::startConsuming(const OrderBook& order_book) {
    is_running_ = true;
    consumer_thread_ = std::thread(&ComputeQueue::run, this, std::cref(order_book));
}

void ComputeQueue::run(const OrderBook& order_book) {
    while (is_running_) {
        std::unique_lock lock(mutex_);
        cv_.wait(lock, [this] {
            return !queue_.empty() || !is_running_;
        });

        if (!queue_.empty()) {
            ArbCheckRequest arb_check_request = queue_.front();
            queue_.pop();

            lock.unlock();

            auto outcomes = order_book.getOddsFor(arb_check_request.event_id, arb_check_request.outcome);

            auto arb = findArbitrage(outcomes);
            if (arb) {
                std::cout << "ARB FOUND: " << arb->event_id << std::endl;
            }
        }
    }
}

std::optional<ArbResponse> ComputeQueue::findArbitrage(const std::vector<OddsUpdate> &odds) {
    if (!odds.empty()) {
        
    }

    return std::nullopt;
}

void ComputeQueue::stop() {
    is_running_ = false;
    cv_.notify_all();
    if (consumer_thread_.joinable()) {
        consumer_thread_.join();
    }
}
