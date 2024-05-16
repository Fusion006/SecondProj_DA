#ifndef SECOND_PROJ_DA_TRIANGULAR_H
#define SECOND_PROJ_DA_TRIANGULAR_H

#include "../Graph.h"
#include "christofides.h"
#include <iostream>
#include <chrono>

vector<int> dfsMST(Graph& g, const int& source);
void dfsVisitMST(Graph& g, Vertex *v, vector<int> & res);
Vertex* getNodeInput(Graph& g, string& order, int& rootV);
vector<int> triangularAproxiamtion(Graph& g, const int& root, double& res);
void printPath(const vector<int>& resVec, const double& res, const int& rootV, chrono::duration<double> duration);
void printTriangularTSPAproximation(Graph& g);
#endif //SECOND_PROJ_DA_TRIANGULAR_H
