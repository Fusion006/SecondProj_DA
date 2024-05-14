#ifndef SECOND_PROJ_DA_CHRISTOFIDES_H
#define SECOND_PROJ_DA_CHRISTOFIDES_H

#include "../Graph.h"
#include <algorithm>
#include <cfloat>

void buildMST(Graph& g);
vector<int> christofides(Graph& g);
void buildEulerTour(Vertex* origin, vector<int>& path);
vector<int> buildTSPtour(vector<int> eulerPath);

#endif
