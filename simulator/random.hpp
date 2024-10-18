#pragma once
#include <random>

class NormalIntGenerator {
   public:
    // Constructor initializes the random engine and distribution
    NormalIntGenerator(double mean = 100000.0, double stddev = 4000.0) : engine_(std::random_device{}()), distribution_(mean, stddev) {}

    // Generate a single random integer following the normal distribution
    inline int operator()() {
        // Generate a floating-point number and round it to the nearest integer
        return static_cast<int>(std::round(distribution_(engine_)));
    }

   private:
    std::mt19937 engine_;                            // Mersenne Twister engine
    std::normal_distribution<double> distribution_;  // Normal distribution
};