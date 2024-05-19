#include <stack>
#include <iostream>
#include "antColOpt.h"
#include "../Graph.h"
#include "../haversine.h"

double getOutgoingDist(Vertex* vertex);
double getTotalWeight(Vertex* vertex);



pair<double,vector<int>> runACO(Graph& g)
{
    g.cleanGraph();
    size_t totalNodes = g.getNumVertex();
    size_t batchSize = 20;
    double numIterations = 100;
    int numNoImprovements = 0;
    double pheromoneDropoff= g.getPheromoneDropoff();

    pair<double, vector<int>> greedyant = runGreedy(g);
    /*
    double bestDistance = greedyant.first;
    vector<int> bestPath = greedyant.second;
    */
    auto bestDistance = DBL_MAX;
    vector<int> bestPath = {};



    for (size_t iteration = 0; iteration < numIterations; iteration++) {
        g.evaporatePheromones(PHEROMONE_PERSISTENCE_RATE);
        stack<Ant> iterationAnts;
        auto bestIterationDistance = DBL_MAX;
        vector<int> bestIterationPath = {};


        for (int i = 0; i < batchSize; i++) {
            Ant ant = getAntPath(g, bestDistance);
            cout<< i<< " " << iteration << " " << numNoImprovements << "\n";
            iterationAnts.push(ant);

            if (ant.distance < bestIterationDistance) {
                bestIterationDistance = ant.distance;
                bestIterationPath = ant.visited;
            }

        }

        if (bestIterationDistance >= bestDistance) {
            numNoImprovements++;
        } else if (bestIterationDistance < bestDistance) {
            numNoImprovements = 0;
            bestDistance = bestIterationDistance;
            bestPath = bestIterationPath;
            cout << "better\n";
        }
        if (numNoImprovements > 10)
            break;



        //Update pheromones
        while (!iterationAnts.empty()) {
            Ant ant = iterationAnts.top();
            iterationAnts.pop();

            double pheromoneDelta = 1 / ant.distance;
            size_t lastPointIndex = ant.visited.size() - 1;
            for (int index = 0; index < lastPointIndex; index++) {
                Edge *edge = g.findEdge(ant.visited[index], ant.visited[index + 1]);
                edge->setPheromones(edge->getPheromones() + pheromoneDelta);
            }
            Edge *edge = g.findEdge(ant.visited[lastPointIndex], 0);
            edge->setPheromones(edge->getPheromones() + pheromoneDelta);
        }
    }
    return {bestDistance,bestPath};

}

Ant getAntPath(Graph& g, double bestDistance){
    double averageDist = g.getPheromoneDropoff();
    //TODO maybe make seperate func to set all unvisited
    for (pair<int,Vertex*> vertex : g.getVertexSet())
    {
        vertex.second->setVisited(false);
    }
    struct Ant ant;
    ant.visited.push_back(0);
    ant.current = g.findVertex(0);
    ant.current->setVisited(true);
    size_t totalNodes = g.getNumVertex();
    while(ant.visited.size() < totalNodes)
    {
        double maxProb = 0;
        vector<Edge*> possibleNeighbors;

        unordered_map<int,Edge*> adjs = ant.current->getAdj();
        for (pair<int,Edge*> neighbor : adjs)
        {
            Vertex* vertex = neighbor.second->getDest();
            if (!vertex->isVisited())
            {
                possibleNeighbors.push_back(neighbor.second);
                maxProb += updateTransitionProbability(neighbor.second, averageDist,getTotalWeight(vertex));
            }
        }

        random_device rnd;
        mt19937 rng(rnd());
        uniform_real_distribution<double> uni(0,maxProb);
        double randomNum = uni(rng);    //Generate random number from 0 to maxProb

        double sumProb = 0;
        for(Edge* neighbor : possibleNeighbors)
        {
            sumProb += neighbor->getTransitionProbability();
            if (sumProb >= randomNum)
            {
                Vertex* dest = neighbor->getDest();
                ant.distance += neighbor->getDistance();
                ant.current = dest;
                ant.visited.push_back(dest->getId());
                dest->setVisited(true);
                break;
            }
        }
    }
    ant.distance += g.findEdge(ant.current->getId(),0)->getDistance();
    return ant;
}

double updateTransitionProbability(Edge* edge, double averageDist, double totalWeight)
{
    double distanceWeight = averageDist / edge->getDistance();
    double pheromonesWeight = edge->getPheromones();
    double newProb = distanceWeight * pheromonesWeight / totalWeight;
    edge->setTransitionProbability(newProb);
    return newProb;
}


pair<double,vector<int>> runGreedy(Graph& g) {
    Ant ant;
    for (pair<int,Vertex*> vertex : g.getVertexSet())
    g.cleanGraph();
    ant.current = g.findVertex(0);
    ant.current->setVisited(true);
    ant.visited.push_back(0);

    while (ant.visited.size() < g.getNumVertex())
    {
        double  minDist = DBL_MAX;
        Vertex* next;
        for (auto p : g.getVertexSet()){
            if (p.second != ant.current && g.findEdge(ant.current->getId(),p.second->getId()) == nullptr)
            {
                double dist = haversine(p.second->getLat(),p.second->getLon(),ant.current->getLat(),ant.current->getLon());
                if (dist < minDist && !p.second->isVisited()) {
                    minDist = dist;
                    next = p.second;
                }
            }
        }
        for (auto p : ant.current->getAdj())
        {
            if (p.second->getDistance() < minDist && !p.second->getDest()->isVisited()) {
                minDist = p.second->getDistance();
                next = p.second->getDest();
            }
        }
        next->setVisited(true);
        ant.visited.push_back(next->getId());
        ant.distance += minDist;
        ant.current = next;
    }
    Edge* returnEdge = g.findEdge(ant.current->getId(),0);
    if (returnEdge == nullptr)
        returnEdge = g.findEdge(0,ant.current->getId());
    ant.distance += returnEdge->getDistance();
    return {ant.distance,ant.visited};
}

double getTotalWeight(Vertex* vertex) {
    double res = 0;
    unordered_map<int,Edge*> adjs = vertex->getAdj();
    for (pair<int,Edge*> neighbor : adjs)
    {
        if (!neighbor.second->getDest()->isVisited())
            res += neighbor.second->getDistance() * neighbor.second->getPheromones();
    }
    return res;
}