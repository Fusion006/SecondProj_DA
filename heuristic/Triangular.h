#ifndef SECOND_PROJ_DA_TRIANGULAR_H
#define SECOND_PROJ_DA_TRIANGULAR_H


/** @defgroup triangular triangular
 * @{
 *
 * Functions and heuristic used to find the optimal path that solves the TSP problem, using a triangular approximation
 */
#include "../Graph.h"
#include "christofides.h"
#include <iostream>
#include <chrono>
#include "../common.h"

using namespace std;

/** @brief Performs a depth-first search (dfs) in the MST of graph @param g from the @param source node.
 * Complexity: O(V + E).
 * @param g MST graph from the original graph selected by user.
 * @param source source node where the traversal starts.
 * @return vector with the nodes id in pre-order order.
 */
vector<int> dfsMST(Graph& g, const int& source);
/** @brief Auxiliary function that visits a vertex @param v and its adjacent, recursively.
 * Updates a parameter with the list of visited node contents.
 * Complexity: O(V + E).
 * @param g MST graph from the original graph selected by user.
 * @param v current node being explored.
 * @param res current order of nodes visited so far.
 */
void dfsVisitMST(Graph& g, Vertex *v, vector<int> & res);
/** @brief Heuristic that tries to find the optimal path for the TSP problem, for starting node @param root.
 * Follows the Triangular approximation algorithm.
 * Complexity: O(V + E).
 * @param g graph selected by the user.
 * @param root starting node.
 * @param res cost of the path.
 * @return vector of integer values, witch represent the nodes, and ordered by the order of the found path.
 */
vector<int> triangularAproxiamtion(Graph& g, const int& root, double& res);
/** @brief Function that calls the Triangular approximation algorithm using graph @param g and prints the found path for the TSP problem, as well as the cost, in seconds, to find the path.
 * Complexity: O(n + V + E).
 * @param g graph selected by the user.
 */
void printTriangularTSPAproximation(Graph& g);


#endif //SECOND_PROJ_DA_TRIANGULAR_H
