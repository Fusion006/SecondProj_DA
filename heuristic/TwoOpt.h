
#ifndef SECOND_PROJ_DA_TWOOPT_H
#define SECOND_PROJ_DA_TWOOPT_H

#include "../common.h"
#include "../Graph.h"
#include <algorithm>
#include <cfloat>
#include <chrono>

using namespace std;

/** @brief Tries to find a path that traverses the graph, using a greedy algorithm.
 * The greedy choice is by choosing the closest node to the current node being explored.
 * Complexity: O(V + E).
 * @param g graph chosen by the user.
 * @param start starting node of the path.
 * @return vector of integer values, that represent the path found.
 */
vector<int> findGreedyTour(Graph& g, const int& start);
/** @brief Applies the 2-opt algorithm to try to improve the given tour into a tour with less cost.
 * It iteratively swaps two edges in the current best tour.
 * The algorithm continues until no further improvement is possible.
 * Comlexity: O(V^4).
 * @param g graph given by the user.
 * @param path vector of integers representing the current tour. It will be changed if a better path is found.
 * @return The cost of the best tour found.
 */
double twoOpt(Graph& g, vector<int>& path);
/** @brief calculates the cost of the given @param path.
 * Complexity: O(V).
 * @param g graph given by the user.
 * @param path path of nodes represented in a vector of integers.
 * @return the value of the cost.
 */
double calculatePathCost(Graph& g, const vector<int>& path);
/** Function that calls the Greedy algorithm using graph @param g and prints the path, if found, for the TSP problem, as well as the cost, in seconds, to find the path.
 * Complexity: O(V).
 * @param g graph given by the user.
 */
void printTwoOptApproximation(Graph& g);


#endif //SECOND_PROJ_DA_TWOOPT_H
