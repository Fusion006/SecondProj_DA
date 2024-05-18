#include "TwoOpt.h"


vector<int> findGreedyTour(Graph& g, const int& start) {
    vector<int> res;
    res.push_back(start);
    g.cleanGraph();
    auto v = g.findVertex(start);
    v->setVisited(true);
    while (res.size() < g.getNumVertex()) {
        Vertex* vNext = nullptr;
        double closest = DBL_MAX;

        for (auto edge : v->getAdj()) {
            if (!edge.second->getDest()->isVisited()) {
                if (edge.second->getDistance() < closest) {
                    closest = edge.second->getDistance();
                    vNext = edge.second->getDest();
                }
            }
        }
        v = vNext;
        if (v == nullptr) {
            res.clear();
            return res;
        }
        res.push_back(v->getId());
        v->setVisited(true);
    }
    if (g.findEdge(v->getId(), start) == nullptr) res.clear();
    return res;
}

double calculatePathCost(Graph& g, const vector<int>& path) {
    auto itStart = path.begin(), itEnd = path.begin() +1;
    double res = 0;

    while (itEnd != path.end()) {
        auto e = g.findEdge(*itStart, *itEnd);
        if (e == nullptr) {
            return DBL_MAX;
        }
        res += e->getDistance();
        itStart++;
        itEnd++;
    }
    auto e = g.findEdge(*itStart, path.at(0));
    if (e == nullptr) return DBL_MAX;
    res += e->getDistance();

    return res;
}

double twoOpt(Graph& g, vector<int>& path) {
    bool improved = true;
    double res = 0;
    while (improved) {
        improved = false;
        res = calculatePathCost(g, path);

        for (size_t i = 1; i < path.size() - 1; ++i) {
            for (size_t j = i + 1; j < path.size(); ++j) {
                vector<int> new_path = path;
                reverse(new_path.begin() + i, new_path.begin() + j);

                double newDist = calculatePathCost(g, new_path);

                if (newDist < res) {
                    path = new_path;
                    res = newDist;
                    improved = true;
                }
            }
        }
    }
    return res;
}

void printTwoOptApproximation(Graph& g) {
    string order;
    Vertex* v = nullptr;
    int start = 0;
    while (v == nullptr) v = getNodeInput(g, order, start);

    vector<int> path;

    auto startTime = chrono::high_resolution_clock::now();

    path = findGreedyTour(g, start);

    if (path.size() < g.getNumVertex()) {
        cout << "Couldn't find Hamilton cycle.." << endl;
        return;
    }

    double res = twoOpt(g, path);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - startTime;

    printPath(path, res, start, duration);
}
