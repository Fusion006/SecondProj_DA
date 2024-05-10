#include "antColOpt.h"

struct ant{
    double bestDistance = DBL_MAX;
    double distance = 0;
    vector<int> visited;
    int totalNodes;
};