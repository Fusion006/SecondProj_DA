#include "Backtracking.h"


void printBacktrackingSolution(Graph& g) {
    string order;
    Vertex* v = nullptr;
    int rootV = 0;
    while (v == nullptr) v = getNodeInput(g, order, rootV);

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

    auto start = chrono::high_resolution_clock::now();

    double res = tspBT(g, n, path, FinalPath, minW, foundSolutionAlready, rootV, 0, 0, rootV);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    if (!foundSolutionAlready) {
        cout << "There is no TSP solution for this graph." << endl;
        return;
    }

    vector<int> resVec;
    for (auto node : FinalPath) {
        resVec.push_back(node);
    }

    printPath(resVec, minW, rootV, duration);
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
        auto edge = g.findEdge(atual, root);
        if (edge == nullptr) return 0;
        double edgeW = edge->getDistance();
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
        auto edge = g.findEdge(atual, i);
        if (edge == nullptr) continue;

        double edgeW = edge->getDistance();

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
