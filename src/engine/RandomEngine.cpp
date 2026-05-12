#include "RandomEngine.h"

RandomEngine::RandomEngine(unsigned int seed):
    generator(seed),
    uniform_dist(0.0, 1.0),
    normal_dist(0.0, 1.0)
{
}

double RandomEngine::uniform() {
    return uniform_dist(generator);
}

double RandomEngine::normal() {
    return normal_dist(generator);
}

int RandomEngine::randint(int min, int max) {
    std::uniform_int_distribution<> dist(min, max);
    return dist(generator);
}


