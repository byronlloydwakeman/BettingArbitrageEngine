//
// Created by 44785 on 03/09/2026.
//

#ifndef BETTINGARBITRAGEENGINE_MOCKMETRICSRECORDER_HPP
#define BETTINGARBITRAGEENGINE_MOCKMETRICSRECORDER_HPP
#include "metricsRecorder.hpp"


class MockMetricsRecorder : public IMetricsRecorder {
public:
    void recordArbFound() override {}
    void observeImpliedSum(double arb_sum) override {}
};


#endif //BETTINGARBITRAGEENGINE_MOCKMETRICSRECORDER_HPP
