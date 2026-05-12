#pragma once
#include <random>

class RandomEngine {
    public:
        RandomEngine(unsigned int seed);
        double uniform();
        double normal();
        int randint(int min, int max);
    private:
        std::mt19937 generator;
        std::uniform_real_distribution<double> uniform_dist;
        std::normal_distribution<double> normal_dist;
    
};

