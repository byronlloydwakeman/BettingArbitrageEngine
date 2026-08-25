//
// Created by 44785 on 22/08/2026.
//

#include "footballComputeQueue.hpp"

#include <algorithm>
#include <iostream>

FootballComputeQueue::FootballComputeQueue() = default;

FootballComputeQueue::~FootballComputeQueue() {
    stop();
}

void FootballComputeQueue::push(const ArbCheckRequest &arb_check_request) {
    std::unique_lock lock(mutex_);
    queue_.push(arb_check_request);
    cv_.notify_all();
}

void FootballComputeQueue::startConsuming(const OrderBook &order_book) {
    is_running_ = true;
    consumer_thread_ = std::thread(&FootballComputeQueue::run, this, std::cref(order_book));
}

void FootballComputeQueue::run(const OrderBook &order_book) {
    while (is_running_) {
        std::unique_lock lock(mutex_);
        cv_.wait(lock, [this] {
            return !queue_.empty() || !is_running_;
        });

        if (!queue_.empty()) {
            ArbCheckRequest arb_check_request = queue_.front();
            queue_.pop();

            lock.unlock();

            auto homewin_outcomes = order_book.getOddsFor(arb_check_request.event_id,
                                                          toString(FootballOutcome::HomeWin));
            auto draw_outcomes = order_book.getOddsFor(arb_check_request.event_id, toString(FootballOutcome::Draw));
            auto awaywin_outcomes = order_book.getOddsFor(arb_check_request.event_id,
                                                          toString(FootballOutcome::AwayWin));

            std::vector<OddsUpdate> combined;
            combined.reserve(homewin_outcomes.size() + draw_outcomes.size() + awaywin_outcomes.size());
            combined.insert(combined.end(), homewin_outcomes.begin(), homewin_outcomes.end());
            combined.insert(combined.end(), draw_outcomes.begin(), draw_outcomes.end());
            combined.insert(combined.end(), awaywin_outcomes.begin(), awaywin_outcomes.end());

            std::vector<ArbResponse> arb = findArbitrage(combined);
            // How to send this to the output feed?
            if (arb.size() > 0) {
                std::cout << "FOUND AN ARBITRAGE: " << std::endl;
                for (auto& a_ : arb) {
                    std::cout << a_ << std::endl;
                }
            }

        }
    }
}

std::vector<ArbResponse> FootballComputeQueue::findArbitrage(const std::vector<OddsUpdate> &odds) {
    if (odds.empty()) {
        return {};
    }

    auto best_home = ArbResponse();
    auto best_draw = ArbResponse();
    auto best_away = ArbResponse();

    // Find the best odds for each outcome to work with
    for (const auto& odd : odds ) {
        if (toEnum(odd.outcome) == FootballOutcome::HomeWin && odd.odds > best_home.odds) {
            best_home = ArbResponse(odd.event_id, odd.bookmaker_id, odd.outcome, odd.odds);
        }
        else if (toEnum(odd.outcome) == FootballOutcome::Draw && odd.odds > best_draw.odds) {
            best_draw =  ArbResponse(odd.event_id, odd.bookmaker_id, odd.outcome, odd.odds);
        }
        else if (toEnum(odd.outcome) == FootballOutcome::AwayWin && odd.odds > best_away.odds) {
            best_away = ArbResponse(odd.event_id, odd.bookmaker_id, odd.outcome, odd.odds);
        }
    }

    // Ensure we have odds for all outcomes
    if (best_home.odds == 0.0 || best_draw.odds == 0.0 || best_away.odds == 0.0 ) {
        return {};
    }

    double implied_sum = (1 / best_home.odds) + (1 / best_draw.odds) + (1 / best_away.odds);

    std::vector<ArbResponse> return_;
    return_.reserve(3);
    if (implied_sum < 1) {
        return_.push_back(best_home); // Home
        return_.push_back(best_away); // Away
        return_.push_back(best_draw); // Draw
        return return_;
    }
    else {
        return {};
    }
}

void FootballComputeQueue::stop() {
    is_running_ = false;
    cv_.notify_all();
    if (consumer_thread_.joinable()) {
        consumer_thread_.join();
    }
}
