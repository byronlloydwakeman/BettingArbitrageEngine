//
// Created by 44785 on 03/09/2026.
//

#include "prometheusMetricRecorder.hpp"

#include <iostream>
#include <ostream>

#include "prometheus/counter.h"
#include "prometheus/histogram.h"

PrometheusMetricRecorder::PrometheusMetricRecorder(std::shared_ptr<prometheus::Registry> registry)
    : registry_(std::move(registry)),
        arbs_found_(prometheus::BuildCounter()
          .Name("arbs_found_total")
          .Help("Total number of arbitrage opportunities found")
          .Register(*registry_)
          .Add({})),
        histogram_(prometheus::BuildHistogram()
        .Name("implied_odds_sum_histogram")
        .Help("Distribution of the bookmakers' implied odds")
        .Register(*registry_)
        .Add({}, std::vector<double>{0.7,0.8,0.9,0.95,1.0,1.05,1.1,1.2,1.3})) {
}

void PrometheusMetricRecorder::recordArbFound() {
    std::cout << "Recorded arb" << std::endl;
    arbs_found_.Increment();
}

void PrometheusMetricRecorder::observeImpliedSum(double arb_sum) {
    histogram_.Observe(arb_sum);
}