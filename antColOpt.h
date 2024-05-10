#ifndef SECOND_PROJ_DA_ANTCOLOPT_H
#define SECOND_PROJ_DA_ANTCOLOPT_H

#include "Graph.h"
#include <cmath>
#include <cfloat>
#include <random>

#define PHEROMONE_PERSISTENCE_RATE 0.95

struct Ant{
    double distance = 0;
    vector<int> visited = {};
    Vertex* current;
};

pair<double,vector<int>> runACO(Graph& g);
pair<double,vector<int>> getAntPath(Graph& g);
double updateTransitionProbability(Edge* edge);
#endif