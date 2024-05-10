#include "antColOpt.h"



pair<double,vector<int>> runACO(Graph& g)
{
    g.cleanGraph();
    size_t totalNodes = g.getNumVertex();
    double batchSize = 1000;//TODO round(totalNodes /2);

    double bestDistance = DBL_MAX;
    vector<int> bestPath = {};

    for(int i = 0; i < batchSize ; i++)
    {
        g.evaporatePheromones(PHEROMONE_PERSISTENCE_RATE);
        pair<double,vector<int>> possiblePath = getAntPath(g);
        double distance = possiblePath.first;
        vector<int> path = possiblePath.second;
        if (distance < bestDistance)
        {
            bestDistance = distance;
            bestPath = path;
        }

        //Update pheromones
        double pheromoneDelta = g.getPheromoneDropoff() / distance;
        size_t lastPointIndex = path.size() - 1;
        for (int index = 0; i < lastPointIndex; i++) {
            Edge *edge = g.findEdge(path[index], path[index + 1]);
            edge->setPheromones(edge->getPheromones() + pheromoneDelta);
        }
    }

    return {bestDistance,bestPath};

}

pair<double,vector<int>> getAntPath(Graph& g){
    //TODO maybe make seperate func
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
                maxProb += updateTransitionProbability(neighbor.second);
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
    return {ant.distance,ant.visited};
}

double updateTransitionProbability(Edge* edge)
{
    double distanceWeight = 1.0 / edge->getDistance();
    double pheromonesWeight = edge->getPheromones();
    double newProb = distanceWeight * pheromonesWeight;
    edge->setTransitionProbability(newProb);
    return newProb;
}