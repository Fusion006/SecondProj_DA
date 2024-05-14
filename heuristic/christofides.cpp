#include <set>
#include <iostream>
#include "christofides.h"

Edge* getMSTEdge(const vector<Vertex*>& tree);
void makeGraphPerfect(set<Vertex*> oddWeightedVertexes, Graph& g);
void buildMST(Graph& g)
{
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
    }
}

vector<int> christofides(Graph& g)
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
    cout<<"Created oddWeightedVertexes\n";

    makeGraphPerfect(oddWeightedVertexes, g);
    cout<<"Made Graph perfect\n";


    vector<int> eulerPath = {0};
    Vertex* origin = g.findVertex(0);
    buildEulerTour(origin,eulerPath);
    cout<<"Made Euler Path\n";

    vector<int> res = buildTSPtour(eulerPath);
    cout<<"Built TSP\n";

    g.eraseCopyEdges();
    cout<<"Cleaned up\n";

    return res;
}

void makeGraphPerfect(set<Vertex*> oddWeightedVertexes, Graph& g)
{
    while (!oddWeightedVertexes.empty())
    {
        Edge* shortestEdge = nullptr;
        auto shortestDistance = DBL_MAX;
        for (Vertex* origin : oddWeightedVertexes)
        {
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
        cout<<oddWeightedVertexes.size()<<endl;
    }
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

void buildEulerTour(Vertex* origin, vector<int>& path) {
    for (pair<int,Edge*> edgePair : origin->getAdj())
    {
        Edge* edge = edgePair.second;
        if (edge->getSelected() && !edge->isUsed())
        {
            edge->setUsed(true);
            edge->getReverse()->setUsed(true);
            path.push_back(edge->getDest()->getId());
            buildEulerTour(edge->getDest(),path);
        }
    }
}

vector<int> buildTSPtour(vector<int> eulerPath) {
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
