//
// Unit tests for FootballComputeQueue::findArbitrage
//

#include <gtest/gtest.h>

#include "footballComputeQueue.hpp"
#include "event.hpp"
#include "footballOutcome.hpp"

namespace {
    std::chrono::system_clock::time_point now() {
        return std::chrono::system_clock::now();
    }

    OddsUpdate makeOdds(const std::string& event_id, FootballOutcome outcome,
                        const std::string& bookmaker_id, double odds) {
        return OddsUpdate(event_id, toString(outcome), bookmaker_id, odds, now());
    }
}

class FindArbitrageTest : public ::testing::Test {
protected:
    FootballComputeQueue queue;
};

TEST_F(FindArbitrageTest, ReturnsEmptyWhenInputIsEmpty) {
    std::vector<OddsUpdate> odds;
    EXPECT_TRUE(queue.findArbitrage(odds).empty());
}

// Shouldn't happen but if the feed doesn't include the odds it should be able to handle it
TEST_F(FindArbitrageTest, ReturnsEmptyWhenAnOutcomeIsMissing) {
    std::vector<OddsUpdate> odds = {
        makeOdds("evt1", FootballOutcome::HomeWin, "bookA", 2.5),
        makeOdds("evt1", FootballOutcome::Draw, "bookA", 3.5),
    };
    EXPECT_TRUE(queue.findArbitrage(odds).empty());
}

// When sum of probs is greater than one
TEST_F(FindArbitrageTest, ReturnsEmptyWhenNoArbitrageExists) {
    std::vector<OddsUpdate> odds = {
        makeOdds("evt1", FootballOutcome::HomeWin, "bookA", 2.0),
        makeOdds("evt1", FootballOutcome::Draw, "bookA", 3.0),
        makeOdds("evt1", FootballOutcome::AwayWin, "bookA", 2.5),
    };
    EXPECT_TRUE(queue.findArbitrage(odds).empty());
}

// When sum of probs is exactly one
TEST_F(FindArbitrageTest, ReturnsEmptyWhenImpliedSumExactlyOne) {
    std::vector<OddsUpdate> odds = {
        makeOdds("evt1", FootballOutcome::HomeWin, "bookA", 2.0),
        makeOdds("evt1", FootballOutcome::Draw, "bookA", 4.0),
        makeOdds("evt1", FootballOutcome::AwayWin, "bookA", 4.0),
    };
    EXPECT_TRUE(queue.findArbitrage(odds).empty());
}

// When sum of probs is less than one
TEST_F(FindArbitrageTest, DetectsArbitrageAcrossBookmakers) {
    std::vector<OddsUpdate> odds = {
        makeOdds("evt1", FootballOutcome::HomeWin, "bookA", 3.0),
        makeOdds("evt1", FootballOutcome::Draw, "bookB", 3.5),
        makeOdds("evt1", FootballOutcome::AwayWin, "bookC", 4.0),
    };

    auto result = queue.findArbitrage(odds);
    ASSERT_EQ(result.size(), 3u);

    // Order returned is [home, away, draw]
    EXPECT_EQ(result[0].outcome, toString(FootballOutcome::HomeWin));
    EXPECT_EQ(result[0].bookmaker_id, "bookA");
    EXPECT_DOUBLE_EQ(result[0].odds, 3.0);

    EXPECT_EQ(result[1].outcome, toString(FootballOutcome::AwayWin));
    EXPECT_EQ(result[1].bookmaker_id, "bookC");
    EXPECT_DOUBLE_EQ(result[1].odds, 4.0);

    EXPECT_EQ(result[2].outcome, toString(FootballOutcome::Draw));
    EXPECT_EQ(result[2].bookmaker_id, "bookB");
    EXPECT_DOUBLE_EQ(result[2].odds, 3.5);

    for (auto& r : result) {
        EXPECT_EQ(r.event_id, "evt1");
    }
}

TEST_F(FindArbitrageTest, SelectsBestOddsPerOutcomeAmongMultipleBookmakers) {
    std::vector<OddsUpdate> odds = {
        makeOdds("evt1", FootballOutcome::HomeWin, "bookA", 2.9),
        makeOdds("evt1", FootballOutcome::HomeWin, "bookB", 3.0), // best home
        makeOdds("evt1", FootballOutcome::Draw, "bookA", 3.2),
        makeOdds("evt1", FootballOutcome::Draw, "bookB", 3.5),   // best draw
        makeOdds("evt1", FootballOutcome::AwayWin, "bookA", 4.0), // best away
        makeOdds("evt1", FootballOutcome::AwayWin, "bookB", 3.9),
    };

    auto result = queue.findArbitrage(odds);
    ASSERT_EQ(result.size(), 3u);

    EXPECT_EQ(result[0].bookmaker_id, "bookB");
    EXPECT_DOUBLE_EQ(result[0].odds, 3.0);

    EXPECT_EQ(result[1].bookmaker_id, "bookA");
    EXPECT_DOUBLE_EQ(result[1].odds, 4.0);

    EXPECT_EQ(result[2].bookmaker_id, "bookB");
    EXPECT_DOUBLE_EQ(result[2].odds, 3.5);
}

// Gracefully handle unknown outcomes
TEST_F(FindArbitrageTest, IgnoresUnrecognizedOutcomeStrings) {
    // An odd with an outcome string that doesn't map to a FootballOutcome should
    // be ignored, and since AwayWin is otherwise missing, no arbitrage is found.
    std::vector<OddsUpdate> odds = {
        makeOdds("evt1", FootballOutcome::HomeWin, "bookA", 3.0),
        makeOdds("evt1", FootballOutcome::Draw, "bookA", 3.5),
        OddsUpdate("evt1", "NotARealOutcome", "bookA", 100.0, now()),
    };
    EXPECT_TRUE(queue.findArbitrage(odds).empty());
}
