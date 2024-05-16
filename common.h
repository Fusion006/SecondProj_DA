#ifndef SECOND_PROJ_DA_COMMON_H
#define SECOND_PROJ_DA_COMMON_H

#include "Graph.h"
#include <iostream>
#include <chrono>
#include <stack>
#include <list>

using namespace std;

Vertex* getNodeInput(Graph& g, string& order, int& rootV);
void printPath(const vector<int>& resVec, const double& res, const int& rootV, chrono::duration<double> duration);
bool checkIfFullyConnected(Graph& g);
void dfs_scc(Graph&g, Vertex *v, stack<int> &s, list<list<int>> &l, int &i);
list<list<int>> sccTarjan(Graph& g);

#endif //SECOND_PROJ_DA_COMMON_H
