#include <set>
#include <iostream>
#include "christofides.h"

void makeGraphPerfect(set<Vertex*> oddWeightedVertexes, Graph& g, Graph& fullGraph);
Graph buildMST(Graph& g)
{
    priority_queue<Edge> edgesQueue;
    for (pair<int,Vertex*> vertex : g.getVertexSet())
    {
        for (pair<int,Edge*> neighbor : vertex.second->getAdj())
        {
            edgesQueue.push(*neighbor.second);
        }
    }
    Graph res;
    g.cleanGraph();
    int treeSize = 0;
    int scc = 0;
    int index = 0;
    while(treeSize < g.getNumVertex() || scc != 1)
    {
        Edge shortestEdge = edgesQueue.top();
        edgesQueue.pop();
        if (shortestEdge.getDest()->isVisited() && shortestEdge.getOrig()->isVisited() && shortestEdge.getDest()->getNum() == shortestEdge.getOrig()->getNum())
            continue;

        Edge* newEdge = g.findEdge(shortestEdge.getOrig()->getId(),shortestEdge.getDest()->getId());
        Vertex *original = newEdge->getOrig();
        Vertex *dest = newEdge->getDest();


        if (original->isVisited() && dest->isVisited()) {
            scc--;
            int originNum = original->getNum();
            int destNum = dest->getNum();
            for (auto pair1 : g.getVertexSet())
            {
                if (pair1.second->getNum() == originNum)
                    pair1.second->setNum(destNum);
            }

        }else if(!original->isVisited() && !dest->isVisited()) {
            original->setVisited(true);
            original->setNum(index);
            dest->setVisited(true);
            dest->setNum(index);

            treeSize += 2;
            index++;
            scc++;
            res.addVertex(original->getId(), original->getName(), original->getLat(), original->getLon());
            res.addVertex(dest->getId(), dest->getName(), dest->getLat(), dest->getLon());

        }else {
            if (!original->isVisited()) {
                treeSize++;
                original->setVisited(true);
                original->setNum(dest->getNum());
                res.addVertex(original->getId(), original->getName(), original->getLat(), original->getLon());
            } else if (!dest->isVisited()) {
                treeSize++;
                dest->setVisited(true);
                dest->setNum(original->getNum());
                res.addVertex(dest->getId(), dest->getName(), dest->getLat(), dest->getLon());
            }

        }

        if (!res.addEdge(original->getId(), dest->getId(), newEdge->getDistance())) {
            cout<<"\nError in building MST\n";
            exit(EXIT_FAILURE);
        }
        if (!res.addEdge(dest->getId(),original->getId(),newEdge->getDistance())) {
            cout<<"\nError in building MST\n";
            exit(EXIT_FAILURE);
        }
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

pair<vector<int>,double> christofides(Graph& g, Graph& fullGraph)
{
    set<Vertex*> oddWeightedVertexes;
    for (pair<int,Vertex*> vertex : g.getVertexSet())
    {
        if (vertex.second->getAdj().size() % 2 != 0)
            oddWeightedVertexes.insert(vertex.second);
    }
    cout<<"making perfect\n";
    makeGraphPerfect(oddWeightedVertexes, g, fullGraph);


    vector<int> eulerPath = {0};
    Vertex* origin = g.findVertex(0);
    buildEulerTour(origin,eulerPath);

    vector<int> res = buildTSPtour(eulerPath);

    g.eraseCopyEdges();
    double dist = 0;
    size_t last = res.size()-1;
    for (int i = 0; i < last; i++){
        Vertex* currPoint = g.findVertex(res[i]);
        Vertex* nextPoint = g.findVertex(res[i+1]);

        Edge* edge = fullGraph.findEdge(currPoint->getId(),nextPoint->getId());
        if (edge == nullptr)
            edge = fullGraph.findEdge(nextPoint->getId(),currPoint->getId());
        dist += edge->getDistance();
    }
    Edge* returnEdge = fullGraph.findEdge(res[last],0);
    if (returnEdge == nullptr){
        returnEdge = fullGraph.findEdge(0,res[last]);
    }
    dist += returnEdge->getDistance();

    return {res,dist};
}

void makeGraphPerfect(set<Vertex*> oddWeightedVertexes, Graph& g, Graph& fullGraph) {
    priority_queue<Edge> edgesQueue;
    for (Vertex* vertex1: oddWeightedVertexes)
    {
        for (Vertex* vertex2: oddWeightedVertexes)
        {
            if (vertex1->getId() == vertex2->getId()) continue;
            Edge* edge = fullGraph.findEdge(vertex1->getId(),vertex2->getId());
            if (edge == nullptr)
                edge = fullGraph.findEdge(vertex2->getId(),vertex1->getId());
            double distance = edge->getDistance();
            Edge newEdge = Edge(vertex1,vertex2, distance);
            Edge newEdge2 = Edge(vertex2,vertex1, distance);

            edgesQueue.push(newEdge);
            edgesQueue.push(newEdge2);

        }
    }

    while (!oddWeightedVertexes.empty()) {
        if (edgesQueue.empty()){
            cout<<"empty";
        }
        Edge edge = edgesQueue.top();
        edgesQueue.pop();
        if (oddWeightedVertexes.find(edge.getOrig()) != oddWeightedVertexes.end() &&
            oddWeightedVertexes.find(edge.getDest()) != oddWeightedVertexes.end()) {

            Edge *shortestEdge = g.findEdge(edge.getOrig()->getId(), edge.getDest()->getId());
            Vertex* shortestEdgeOrigin = edge.getOrig();
            Vertex* shortestEdgeDest = edge.getDest();

            if (shortestEdge == nullptr) {
                //there are no edges that connect those 2 points
                shortestEdgeOrigin->addEdge(shortestEdgeDest, edge.getDistance());
                shortestEdgeDest->addEdge(shortestEdgeOrigin, edge.getDistance());
            } else {
                //there is already 1 edge that connects those 2 points soo we need a copyEdge
                shortestEdgeOrigin->addCopyEdge(shortestEdge);
            }

            oddWeightedVertexes.erase(shortestEdgeOrigin);
            oddWeightedVertexes.erase(shortestEdgeDest);
        }
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