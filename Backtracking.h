#ifndef SECOND_PROJ_DA_BACKTRACKING_H
#define SECOND_PROJ_DA_BACKTRACKING_H

#include "Graph.h"
#include <iostream>
#include <climits>
#include <cmath>
#include <cfloat>
#include <chrono>
using namespace std;


/**
 * Function that calls the Backtracking algorithm using graph g and prints the result.
 * Prints the cost of the resulting path and each node that belongs to that path.
 * @param g graph selected by the user.
 */
void printBacktrackingSolution(Graph& g);

/**
 * Function that uses a Backtracking algorithm to find the minimum cost path around the graph g.
 * Calculates the minimum path processing every path possibility in the graph. If the cost is higher than the previous one, discards the current possibility.
 * @param g graph selected by the user containing the distance between each node.
 * @param n number of nodes in the graph g.
 * @param path array to store the path being explored.
 * @param finalPath array to store the final path (optimal path).
 * @param minWeight variable that stores the minimum distance stored so far.
 * @param foundASolutionAlready flag indicating to see if whether there is already a solution.
 * @param atual current node being visited.
 * @param index the index of path array where the current node should be placed.
 * @param curentWeight the current weight of the path being explored.
 * @param root starting node.
 * @return the value of the minimum path.
 */
double tspBT(Graph& g, unsigned int n, unsigned int path[], unsigned int finalPath[], double & minWeight, bool& foundASolutionAlready, unsigned int atual, unsigned int index, double curentWeight, unsigned int root);


#endif //SECOND_PROJ_DA_BACKTRACKING_H
