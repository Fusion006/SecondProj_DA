#ifndef SECOND_PROJ_DA_CHRISTOFIDES_H
#define SECOND_PROJ_DA_CHRISTOFIDES_H

/** @defgroup  Christofides Christofides
 * @{
*
* Functions used to find the optimal path that solves the TSP problem, using the Christofides algorithm
*/
#include "../Graph.h"
#include "../common.h"
#include "../haversine.h"
#include <algorithm>
#include <cfloat>

/**
 * @brief builds a subgraph that represents the minimum spanning tree of g
 * @param g  Graph to calculate MST
 * @return the MST of g
 * Complexity O(E^3)
 */
Graph buildMST(Graph& g);
/**
 * @brief Applies the Christofides algorithm to the graph
 * @param g Graph representing the MST of our graph
 * @param fullGraph Graph that represents the whole graph of the MST for edge length calculations
 * @return pair with vector of vertexes in the TSP path and its distance
 * Complexity O(E^3)
 */
pair<vector<int>,double> christofides(Graph& g, Graph& fullGraph);

/**
 * @brief Calculates an Euler path from a MST
 * @param origin Vertex that will be the origin of our path ant that belongs to a MST
 * @param path Path that represents the vertexes in the Euler path
 * Complexity O(V + E)
 */
void buildEulerTour(Vertex* origin, vector<int>& path);

/**
 * @brief Make a TSP tour using an euler Path
 * @param eulerPath the euler Path that will be turned to a TSP tour
 * @return Vector that represents the computed TSP tour
 * Complexity O(n)
 */
vector<int> buildTSPtour(const vector<int>& eulerPath);

/**
 * @brief Algorithm that applies the 2k-opt to a TSP tour
 * @param res TSP tour that will be optimized
 * @param fullGraph Graph that represents the whole graph of the MST for edge length calculations
 * @return the length of the final TSP tour
 * Complexity O(n^3)
 */
double run2opt(vector<int>& res, Graph& fullGraph);

#endif
