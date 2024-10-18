#pragma once
#include <cstdint>
#include <random>

class ClampedNormalIntGenerator {
   public:
    ClampedNormalIntGenerator(double mean = 100000.0, double stddev = 4000.0, int min_value = 0)
        : engine_(std::random_device{}()), distribution_(mean, stddev), min_value_(min_value) {}

    // Generate a single non-negative random integer following the normal distribution
    inline uint32_t operator()() {
        auto sample = distribution_(engine_);
        // Clamp the sample to the minimum value
        sample = std::max(sample, static_cast<double>(min_value_));
        return static_cast<uint32_t>(std::round(sample));
    }

   private:
    std::mt19937 engine_;
    std::normal_distribution<double> distribution_;
    int min_value_;
};