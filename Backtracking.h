#ifndef SECOND_PROJ_DA_BACKTRACKING_H
#define SECOND_PROJ_DA_BACKTRACKING_H

#include "Graph.h"
#include <iostream>
#include <climits>
using namespace std;

//void backtrack(Graph& g, int currentVertex, double currentPath, vector<int>& currentPathList, double& minPath, vector<int>& path);

void printBacktrackingSolution(Graph& g);

double tspBT(Graph& g, unsigned int n, unsigned int path[], double & minWeight, bool& foundASolutionAlready, unsigned int atual, unsigned int index, double curentWeight, unsigned int root);


#endif //SECOND_PROJ_DA_BACKTRACKING_H
