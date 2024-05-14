#include "Backtracking.h"
/*
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
}*/


void printBacktrackingSolution(Graph& g) {
    string order;
    cout << "What is the root node?" << endl;
    getline(cin >> ws, order);
    if (order[0] < '0' || order[0] > '9') {
        cout << "Invalid node!" << endl;
        return;
    }
    int rootV = stoi(order);
    auto v = g.findVertex(rootV);

    if (v == nullptr) {
        cout << "The node selected does not exist!" << endl;
        return;
    }

    unsigned int n = g.getNumVertex();
    unsigned int path[n];
    for (int i = 0; i < n; i++) {
        path[i] = rootV;
    }
    unsigned int FinalPath[n];
    for (int i = 0; i < n; i++) {
        path[i] = rootV;
    }
    double minW = DBL_MAX;
    bool foundSolutionAlready = false;
    double res = tspBT(g, n, path, FinalPath, minW, foundSolutionAlready, rootV, 0, 0, rootV);

    if (!foundSolutionAlready) {
        cout << "There is no solution for this graph." << endl;
        return;
    }

    cout << "There is a path with cost " << minW << " for this graph:" << endl;

    for (auto node : FinalPath) {
        cout << " " << node << " ==>";
    }
    cout << " " << rootV << endl;
}

double tspBT(Graph& g, unsigned int n, unsigned int path[], unsigned int finalPath[], double & minWeight, bool& foundASolutionAlready, unsigned int atual, unsigned int index, double curentWeight, unsigned int root) {
    //adicionar ao path o node atual
    path[index] = atual;
    unsigned int resPath[n];
    for (int i = 0; i < n; i++) {
        resPath[i] = path[i];
    }
    double res = 0;

    //voltar ao 0
    if (index == n - 1) {
        double edgeW = g.findEdge(atual, root)->getDistance();
        if ((curentWeight + edgeW) < minWeight) {
            minWeight = curentWeight + edgeW;
            foundASolutionAlready = false;
            for (int i = 0; i < n; i++) {
                finalPath[i] = path[i];
            }
        }
        return edgeW;
    }

    //ver cada opcao valida
    for (int i = 0; i < n; i++) {
        if (i == atual || i == root) continue;

        //ver se ja foi visitado
        bool visited = false;
        for (int j = 0; j < n; j++) {
            if (path[j] == i) visited = true;
        }
        if (visited) {
            continue;
        }

        //bounding
        double edgeW = g.findEdge(atual, i)->getDistance();

        if (abs((curentWeight + edgeW) - minWeight) < 0.0000001 || (curentWeight + edgeW) < minWeight) {
            unsigned int updatedPath[n];
            for (int j = 0; j < n; j++) {
                updatedPath[j] = path[j];
            }

            double possibleRes = edgeW + tspBT(g, n, updatedPath, finalPath, minWeight, foundASolutionAlready, i, index+1, curentWeight + edgeW, root);

            if ((abs(possibleRes - (minWeight - curentWeight)) < 0.0000001) && !foundASolutionAlready) {
                if (atual == root) foundASolutionAlready = true;
                for (int j = 0; j < n; j++) {
                    resPath[j] = updatedPath[j];
                    res = possibleRes;
                }
            }
        }
    }
    for (int j = 0; j < n; j++) {
        path[j] = resPath[j];
    }
    return res;
}
