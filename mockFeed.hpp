//
// Created by 44785 on 31/07/2026.
//

#ifndef BETTINGARBITRAGEENGINE_MOCKFEED_HPP
#define BETTINGARBITRAGEENGINE_MOCKFEED_HPP
#include <condition_variable>
#include <thread>

#include "feed.hpp"


class MockFeed : public IFeed {
public:
    explicit MockFeed(std::string bookmaker_id);
    void connect() override;
    void setCallback(std::function<void(const OddsUpdate &)> callback) override;
    void disconnect() override;
    ~MockFeed() override;

    static std::vector<double> generateTrueProbabilities(int n);
    static std::vector<double> makeBookmakerOdds(const std::vector<double>& probs, double marginFactor);
    static double generateMargin(double min = 1.02, double max = 1.12);

private:
    std::string bookmaker_id_;
    std::function<void(const OddsUpdate &)> callback_;
    std::thread worker_;
    std::atomic<bool> is_running_{false};

    void run();
};

#endif //BETTINGARBITRAGEENGINE_MOCKFEED_HPP
