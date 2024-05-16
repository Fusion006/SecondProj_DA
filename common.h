#ifndef SECOND_PROJ_DA_COMMON_H
#define SECOND_PROJ_DA_COMMON_H

#include "Graph.h"
#include <iostream>
#include <chrono>

using namespace std;

Vertex* getNodeInput(Graph& g, string& order, int& rootV);
void printPath(const vector<int>& resVec, const double& res, const int& rootV, chrono::duration<double> duration);

#endif //SECOND_PROJ_DA_COMMON_H
