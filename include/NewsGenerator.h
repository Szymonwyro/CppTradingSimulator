#pragma once
#include <vector>
#include "NewsEvent.h"
#include "RandomEngine.h"

class NewsGenerator {
    public:
        std::vector<NewsEvent> generateNews(int horizon, RandomEngine& rng);

};