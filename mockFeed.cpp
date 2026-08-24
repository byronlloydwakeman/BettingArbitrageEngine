#include "mockFeed.hpp"
#include <random>
#include <vector>
#include <string>
#include <iostream>
#include <format>

MockFeed::MockFeed(std::string bookmaker_id)
    : bookmaker_id_{std::move(bookmaker_id)} {
}

MockFeed::~MockFeed() {
    disconnect();
}

const std::vector<std::vector<std::vector<std::string> > > outcomes = {
    {
        {"Arsenal | Chelsea", FOOTBALL_OUTCOMES},
        {"Arsenal | Chelsea", "Draw"},
        {"Arsenal | Chelsea", "Away Win"}
    },
    {
        {"Manchester United | Liverpool", "Home win"},
        {"Manchester United | Liverpool", "Draw"},
        {"Manchester United | Liverpool", "Away Win"}
    }
};

void MockFeed::connect() {
    is_running_ = true;
    worker_ = std::thread(&MockFeed::run, this);
}

void MockFeed::disconnect() {
    is_running_ = false;
    std::cout << "Disconnecting..." << std::endl;

    if (worker_.joinable()) {
        worker_.join();
    }
}

void MockFeed::setCallback(std::function<void(const OddsUpdate &)> callback) {
    callback_ = std::move(callback);
}

void MockFeed::run() {
    while (is_running_) {
        // Randomly pick a market set
        static thread_local std::mt19937 gen(std::random_device{}());
        std::uniform_int_distribution<> dist(0, outcomes.size() - 1);

        const auto &market = outcomes[dist(gen)];

        // Generate true probabilities
        std::vector<double> probs = generateTrueProbabilities(market.size());

        // Generate margin + odds
        double margin = generateMargin();
        std::vector<double> odds = makeBookmakerOdds(probs, margin);

        // Build feed updates
        for (size_t i = 0; i < market.size(); i++) {
            auto feed = std::make_unique<OddsUpdate>(
                market[i][0],
                market[i][1],
                bookmaker_id_,
                odds[i],
                std::chrono::system_clock::now()
            );

            if (callback_) {
                callback_(*feed);
            }
        }
    }
}

/**
 * Generates n number of probabilities than summate to less than 1
 * @param n Number of odds to be generated than summate to less than 1
 * @return Vector array with n length probabilities
 */
std::vector<double> MockFeed::generateTrueProbabilities(int n) {
    thread_local std::mt19937 gen(std::random_device{}());
    std::gamma_distribution gamma(1.0);

    std::vector<double> values(n);
    double sum = 0.0;

    for (int i = 0; i < n; i++) {
        values[i] = gamma(gen);
        sum += values[i];
    }
    for (int i = 0; i < n; i++) {
        values[i] /= sum;
    }
    return values;
}

/**
 * Turns the Vector array of 'true' probabilities into odds that bookmakers are likely to set
 * @param probs Vector array of 'true' probabilities
 * @param marginFactor Profit margin of which is set by the bookmaker
 * @return Vector array of bookmaker odds for a given market set of probabilities
 */
std::vector<double> MockFeed::makeBookmakerOdds(const std::vector<double> &probs, double marginFactor) {
    std::vector<double> odds;
    odds.reserve(probs.size());

    for (double p: probs) {
        double implied = p * marginFactor;
        odds.push_back(1.0 / implied);
    }
    return odds;
}

/**
 * Utility method to randomly generate a profit margin for a bookmaker
 * @param min Minimum margin, default is 2%
 * @param max Maximum margin, default is 12%
 * @return Double within the given min - max range
 */
double MockFeed::generateMargin(double min, double max) {
    thread_local std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution dist(min, max);
    return dist(gen);
}
