#ifndef SECOND_PROJ_DA_ANTCOLOPT_H
#define SECOND_PROJ_DA_ANTCOLOPT_H

#include "../Graph.h"
#include <cmath>
#include <cfloat>
#include <random>

#define PHEROMONE_PERSISTENCE_RATE 0.95

struct Ant{
    double distance = 0;
    vector<int> visited = {};
    Vertex* current;
};

/**
 * @brief Unused Ant Colony Optimization algorithm
 * @param g Complete Graph
 * @return distance of best found path
 */
pair<double,vector<int>> runACO(Graph& g);

/**
 *
 * @param g Complete Graph
 * @param bestDistance best distance found in the graph until now
 * @return Ant that has its Path and Distance travelled information
 */
Ant getAntPath(Graph& g, double bestDistance);

/**
 *
 * @param edge Edge that we will be updating Transition Probability
 * @param averageDist the average distance in the graph
 * @param totalWeight the "total probability" of that 'edge' origin vertex
 * @return the new probability of 'edge'
 */
double updateTransitionProbability(Edge* edge, double averageDist, double totalWeight);

/**
 *
 * @param g Graph that will be calculated
 * @return Runs a simple nearest Neighbor approach to the TSP for comparison with the ACO (not good for very big graphs)
 */
pair<double,vector<int>> runGreedy(Graph& g);
#endif