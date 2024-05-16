#ifndef SECOND_PROJ_DA_TRIANGULAR_H
#define SECOND_PROJ_DA_TRIANGULAR_H

#include "../Graph.h"
#include "christofides.h"
#include <iostream>
#include <chrono>
#include "../common.h"

using namespace std;

vector<int> dfsMST(Graph& g, const int& source);
void dfsVisitMST(Graph& g, Vertex *v, vector<int> & res);
vector<int> triangularAproxiamtion(Graph& g, const int& root, double& res);
void printTriangularTSPAproximation(Graph& g);
#endif //SECOND_PROJ_DA_TRIANGULAR_H
