#include <set>
#include <iostream>
#include "christofides.h"

Edge* getMSTEdge(const vector<Vertex*>& tree);
void makeGraphPerfect(set<Vertex*> oddWeightedVertexes, Graph& g);
Graph buildMST(Graph& g)
{
    Graph res;
    g.cleanGraph();
    vector<Vertex*> tree;
    Vertex* origin = g.findVertex(0);
    origin->setVisited(true);
    tree.push_back(origin);

    while(tree.size() < g.getNumVertex())
    {
        Edge* newEdge = getMSTEdge(tree);
        Edge* reverseEdge = g.findEdge(newEdge->getDest()->getId(),newEdge->getOrig()->getId());
        newEdge->getDest()->setVisited(true);
        newEdge->setSelected(true);
        reverseEdge->setSelected(true);
        tree.push_back(newEdge->getDest());
        Vertex *original = newEdge->getOrig();
        Vertex *dest = newEdge->getDest();
        res.addVertex(original->getId(),original->getName(),original->getLat(),original->getLon());
        res.addVertex(dest->getId(),dest->getName(),dest->getLat(),dest->getLon());
        res.addEdge(original->getId(),dest->getId(),newEdge->getDistance());
        res.addEdge(dest->getId(),original->getId(),newEdge->getDistance());
    }
    return res;
}

Edge* getMSTEdge(const vector<Vertex*>& tree)
{
    Edge* res = nullptr;
    auto minDist = DBL_MAX;
    for (Vertex* vertex : tree)
    {
        for (pair<int,Edge*> edge : vertex->getAdj())
        {
            if (!edge.second->getDest()->isVisited() && edge.second->getDistance() < minDist)
            {
                res = edge.second;
                minDist = edge.second->getDistance();
            }
        }
    }
    return res;
}

pair<vector<int>,double> christofides(Graph& g)
{
    set<Vertex*> oddWeightedVertexes;
    for (pair<int,Vertex*> vertex : g.getVertexSet())
    {
        int numEdgesInTree = 0;
        for (pair<int,Edge*> edge : vertex.second->getAdj())
        {
            if (!edge.second->getSelected())
            {
                numEdgesInTree++;
            }
        }
        if (numEdgesInTree % 2 != 0)
            oddWeightedVertexes.insert(vertex.second);
    }
    cout<<"making perfect\n";
    makeGraphPerfect(oddWeightedVertexes, g);


    vector<int> eulerPath = {0};
    Vertex* origin = g.findVertex(0);
    buildEulerTour(origin,eulerPath);

    vector<int> res = buildTSPtour(eulerPath);

    g.eraseCopyEdges();
    double dist = 0;
    size_t last = res.size()-1;
    for (int i = 0; i < last; i++){
        dist += g.findEdge(eulerPath[i],eulerPath[i+1])->getDistance();
    }


    return {res,dist};
}

void makeGraphPerfect(set<Vertex*> oddWeightedVertexes, Graph& g)
{
    while (!oddWeightedVertexes.empty())
    {
        Edge* shortestEdge = nullptr;
        auto shortestDistance = DBL_MAX;
        Edge* edge;
        double distance;
        for (Vertex* origin : oddWeightedVertexes)
        {
            for (Vertex* neighbor : oddWeightedVertexes){
                edge = g.findEdge(neighbor->getId(),origin->getId());
                if ((edge == nullptr ) && (neighbor->getId() != origin->getId())){
                    distance = haversine(origin->getLat(),origin->getLon(),neighbor->getLat(),neighbor->getLon());
                    neighbor->addEdge(origin,distance);
                    origin->addEdge(neighbor,distance);
                }
            }
            for (Vertex* dest : oddWeightedVertexes)
            {
                if (dest == origin) continue;
                Edge* newEdge = g.findEdge(origin->getId(),dest->getId());
                if (shortestDistance > newEdge->getDistance())
                {
                    shortestDistance = newEdge->getDistance();
                    shortestEdge = newEdge;
                }
            }
        }
        Vertex* origin = shortestEdge->getOrig();
        Vertex* dest = shortestEdge->getDest();

        Edge* edgeCopy = origin->addCopyEdge(shortestEdge);
        edgeCopy->setSelected(true);
        edgeCopy->getReverse()->setSelected(true);

        oddWeightedVertexes.erase(origin);
        oddWeightedVertexes.erase(dest);
        //oddWeightedVertexes.erase( (oddWeightedVertexes.begin(),oddWeightedVertexes.end(),origin),oddWeightedVertexes.end());
        //oddWeightedVertexes.erase(remove(oddWeightedVertexes.begin(),oddWeightedVertexes.end(),dest),oddWeightedVertexes.end());
    }
}

void buildEulerTour(Vertex* origin, vector<int>& path) {
    for (pair<int,Edge*> edgePair : origin->getAdj())
    {
        Edge* edge = edgePair.second;
        if (!edge->isUsed())
        {
            edge->setUsed(true);
            edge->getReverse()->setUsed(true);
            path.push_back(edge->getDest()->getId());
            buildEulerTour(edge->getDest(),path);
        }
    }
}

vector<int> buildTSPtour(const vector<int>& eulerPath) {
    vector<int> res;
    set<int> visited;
    for (int vertex : eulerPath)
    {
        if (visited.find(vertex) == visited.end())
        {
            visited.insert(vertex);
            res.push_back(vertex);
        }
    }
    return res;
}