//
// Created by 44785 on 03/09/2026.
//

#ifndef BETTINGARBITRAGEENGINE_METRICSRECORDER_HPP
#define BETTINGARBITRAGEENGINE_METRICSRECORDER_HPP


class IMetricsRecorder {
public:
    virtual ~IMetricsRecorder() = default;
    virtual void recordArbFound() = 0;
    virtual void observeImpliedSum(double arb_sum) = 0;
};


#endif //BETTINGARBITRAGEENGINE_METRICSRECORDER_HPP
