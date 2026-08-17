//
// Created by 44785 on 02/08/2026.
//

#ifndef BETTINGARBITRAGEENGINE_INGESTION_HPP
#define BETTINGARBITRAGEENGINE_INGESTION_HPP
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>

#include "event.hpp"


class IngestionQueue {
public:
    IngestionQueue();
    ~IngestionQueue();

    void push(const OddsUpdate& update);
    void startConsuming(std::function<void(const OddsUpdate&)> handler);
    void stop();

private:
    std::queue<OddsUpdate> queue_;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::atomic<bool> is_running_{false};
    std::thread consumer_thread_;

    void consumeLoop(std::function<void(OddsUpdate&)> handler);
};


#endif //BETTINGARBITRAGEENGINE_INGESTION_HPP
