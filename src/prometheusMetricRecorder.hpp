//
// Created by 44785 on 03/09/2026.
//

#ifndef BETTINGARBITRAGEENGINE_PROMETHEUSMETRICRECORDER_HPP
#define BETTINGARBITRAGEENGINE_PROMETHEUSMETRICRECORDER_HPP
#include <memory>

#include "metricsRecorder.hpp"
#include "prometheus/registry.h"


class PrometheusMetricRecorder : public IMetricsRecorder {
public:
    explicit PrometheusMetricRecorder(std::shared_ptr<prometheus::Registry> registry);

    void recordArbFound() override;
    void observeImpliedSum(double arb_sum) override;
private:
    std::shared_ptr<prometheus::Registry> registry_;
    prometheus::Counter& arbs_found_;
    prometheus::Histogram& histogram_;
};


#endif //BETTINGARBITRAGEENGINE_PROMETHEUSMETRICRECORDER_HPP
