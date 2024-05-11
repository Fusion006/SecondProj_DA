#include "Backtracking.h"

void backtrack(Graph& g, int currentVertex, double currentPath, vector<int>& currentPathList, double& minPath, vector<int>& path){
    if(currentPath >= minPath) return;
    currentPathList.push_back(currentVertex);
    if(currentPathList.size() == g.getVertexSet().size()){
        if(g.findPipe(currentVertex, 0) != nullptr){
            currentPath += g.findPipe(currentVertex, 0)->getDistance();
            if(currentPath + g.findPipe(currentVertex, 0)->getDistance() < minPath){
                minPath = currentPath;
                path = currentPathList;
            }
            currentPath -= g.findPipe(currentVertex, 0)->getDistance();
        }
        currentPathList.pop_back();
        return;
    }

    g.findVertex(currentVertex)->setVisited(true);
    for(const auto& [id, neighbor] : g.getVertexSet()){
        if(!neighbor->isVisited() && g.findPipe(currentVertex, id) != nullptr){
            backtrack(g, id, currentPath + g.findPipe(currentVertex, id)->getDistance(), currentPathList, minPath, path);
        }
    }
    g.findVertex(currentVertex)->setVisited(false);
    currentPathList.pop_back();
}
double tspBT(Graph& g){
    vector<int> path;
    double minPath = INT_MAX;
    for(const auto& [id, vertex]: g.getVertexSet()){
        vertex->setVisited(false);
    }
    for(const auto& [id, vertex]: g.getVertexSet()) backtrack(g, id, 0, path, minPath, path);
    for(auto i : path) cout << i << endl;
    return minPath;
}

