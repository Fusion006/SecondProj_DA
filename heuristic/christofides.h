#ifndef SECOND_PROJ_DA_CHRISTOFIDES_H
#define SECOND_PROJ_DA_CHRISTOFIDES_H

#include "../Graph.h"
#include "../common.h"
#include "../haversine.h"
#include <algorithm>
#include <cfloat>

Graph buildMST(Graph& g);
pair<vector<int>,double> christofides(Graph& g, Graph& fullGraph);
void buildEulerTour(Vertex* origin, vector<int>& path);
vector<int> buildTSPtour(const vector<int>& eulerPath);

#endif
