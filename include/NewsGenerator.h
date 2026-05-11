#pragma once
#include <vector>
#include "NewsEvent.h"

class NewsGenerator {
    public:
        std::vector<NewsEvent> generateNews(int horizon);

};