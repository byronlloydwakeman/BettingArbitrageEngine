//
// Created by 44785 on 02/08/2026.
//

#include "ingestion.hpp"

#include <iostream>
#include <utility>

IngestionQueue::IngestionQueue() = default;


IngestionQueue::~IngestionQueue() {
    stop();
}

void IngestionQueue::push(const OddsUpdate &update) {
    std::unique_lock lock(mutex_);
    queue_.push(update);
    cv_.notify_all();
}

void IngestionQueue::startConsuming(std::function<void(const OddsUpdate &)> handler) {
    is_running_ = true;
    std::cout << "Starting the ingestion queue" << std::endl;
    consumer_thread_ = std::thread(&IngestionQueue::consumeLoop, this, std::move(handler));
}

void IngestionQueue::stop() {
    is_running_ = false;
    cv_.notify_all();
    std::cout << "Stopping the ingestion queue" << std::endl;

    if (consumer_thread_.joinable()) {
        consumer_thread_.join();
    }
}

void IngestionQueue::consumeLoop(std::function<void(OddsUpdate&)> handler) {
    while (is_running_) {
        std::unique_lock lock(mutex_);
        cv_.wait(lock, [this] {
            return !queue_.empty() || !is_running_;
        });

        if (!queue_.empty()) {
            OddsUpdate update = queue_.front();
            queue_.pop();
            lock.unlock();

            // Pass through to the callback that handles the OddsUpdate instance
            handler(update);
        }


    }
}
