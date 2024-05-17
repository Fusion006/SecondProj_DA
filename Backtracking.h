#ifndef SECOND_PROJ_DA_BACKTRACKING_H
#define SECOND_PROJ_DA_BACKTRACKING_H


/** @defgroup  backtracking backtracking
 * @{
 *
 * Functions used to find the optimal path that solves the TSP problem, using a backtracking algorithm
 */
#include "Graph.h"
#include "common.h"
#include "heuristic/Triangular.h"
#include <iostream>
#include <climits>
#include <cmath>
#include <cfloat>
#include <chrono>

using namespace std;


/**
 * @brief Function that calls the Backtracking algorithm using graph @param g and prints the path found path for the TSP problem, as well as the cost, in seconds, to find the path.
 * Complexity: O(n + 2^n).
 * @param g graph selected by the user.
 */
void printBacktrackingSolution(Graph& g);

/**
 * @brief Function that uses a Backtracking algorithm to find the minimum cost path around the graph g.
 * Recursively calculates the minimum path processing every path possibility in the graph, but it prunes a non explored path if the cost is higher than the minimum cost found so far.
 * Complexity: O(2^n).
 * @param g graph selected by the user containing the distance between each node.
 * @param n number of nodes in the graph g.
 * @param path array to store the path currently being explored.
 * @param finalPath array to store the final path (optimal path).
 * @param minWeight variable that stores the minimum path cost found so far.
 * @param foundASolutionAlready flag indicating to see if whether there is already a solution.
 * @param atual current node being visited.
 * @param index the index of path array where the current node should be placed.
 * @param curentWeight the current weight of the path being explored.
 * @param root starting node.
 * @return the value of the minimum path.
 */
double tspBT(Graph& g, unsigned int n, unsigned int path[], unsigned int finalPath[], double & minWeight, bool& foundASolutionAlready, unsigned int atual, unsigned int index, double curentWeight, unsigned int root);


#endif //SECOND_PROJ_DA_BACKTRACKING_H
