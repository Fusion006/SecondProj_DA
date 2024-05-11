#ifndef SECOND_PROJ_DA_BACKTRACKING_H
#define SECOND_PROJ_DA_BACKTRACKING_H

#include "Graph.h"
#include <iostream>
using namespace std;

void backtrack(Graph& g, int currentVertex, double currentPath, vector<int>& currentPathList, double& minPath, vector<int>& path);

double tspBT(Graph& g);


#endif //SECOND_PROJ_DA_BACKTRACKING_H
