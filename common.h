#ifndef SECOND_PROJ_DA_COMMON_H
#define SECOND_PROJ_DA_COMMON_H


/** @defgroup common common
 * @{
 *
 * Auxiliary functions utilized by many different functions
 */
#include "Graph.h"
#include <iostream>
#include <chrono>
#include <stack>
#include <set>

using namespace std;

/** @brief Asks and validates the user input for the starting node.
 * Each time it is called, it asks the user for the node, and then validates the input by searching the graph for the node.
 * If the node is not found, a nullptr is returned.
 * Complexity: O(1).
 * @param g graph that contains(or not) the given input root node
 * @param order string that is assigned the user input value
 * @param rootV integer variable that is assigned the valid root node.
 * @return Either the pointer of the root node found in the graph, or a nullptr to indicate that the node was not found.
 */
Vertex* getNodeInput(Graph& g, string& order, int& rootV);
/** @brief Prints the vertices and the total cost of a path that traverses the a graph, in a user friendly way.
 * Prints the cost, and then the path, ending with node @param rootV followed by the duration of the search.
 * Complexity: O(n).
 * @param resVec vector of integer values, witch represent the nodes, and ordered by the order of the found path.
 * @param res cost of the path found
 * @param rootV the starting(and ending) node of the traversal.
 * @param duration duration of the search for path in seconds.
 */
void printPath(const vector<int>& resVec, const double& res, const int& rootV, chrono::duration<double> duration);
/**
 *
 * @param g
 * @return
 */
bool checkIfFullyConnected(Graph& g);
/**
 *
 * @param g
 * @param v
 * @param s
 * @param l
 * @param i
 */
void dfs_scc(Graph&g, Vertex *v, stack<int> &s, vector<set<int>> &l, int &i);
/**
 *
 * @param g
 * @return
 */
vector<set<int>> sccTarjan(Graph& g);

#endif //SECOND_PROJ_DA_COMMON_H
