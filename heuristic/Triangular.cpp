#include "Triangular.h"


/*
 * Performs a depth-first search (dfs) in a graph (this) from the source node.
 * Returns a vector with the contents of the vertices by dfs order.
 */
vector<int> dfsMST(Graph& g, const int& source) {
    vector<int> res;
    // Get the source vertex
    auto s = g.findVertex(source);
    if (s == nullptr) {
        return res;
    }
    // Set that no vertex has been visited yet
    for (auto v : g.getVertexSet()) {
        v.second->setVisited(false);
    }
    // Perform the actual DFS using recursion
    dfsVisitMST(g, s, res);

    return res;
}

/*
 * Auxiliary function that visits a vertex (v) and its adjacent, recursively.
 * Updates a parameter with the list of visited node contents.
 */
void dfsVisitMST(Graph& g, Vertex *v, vector<int> & res) {
    v->setVisited(true);
    res.push_back(v->getId());
    for (auto & e : v->getAdj()) {
        if (!e.second->getSelected()) continue;
        auto w = e.second->getDest();
        if (!w->isVisited()) {
            dfsVisitMST(g, w, res);
        }
    }
}

Vertex* getNodeInput(Graph& g, string& order, int& rootV) {
    cout << "What is the root node?" << endl;
    getline(cin >> ws, order);
    if (order[0] < '0' || order[0] > '9') {
        cout << "Invalid node!" << endl;
        return nullptr;
    }
    rootV = stoi(order);
    auto v = g.findVertex(rootV);

    if (v == nullptr) {
        cout << "The node selected does not exist!" << endl;
        return nullptr;
    }
    return v;
}

vector<int> triangularAproxiamtion(Graph& g, const int& root, double& res) {
    buildMST(g);
    vector<int> resVec = dfsMST(g, root);
    auto itStart = resVec.begin(), itEnd = resVec.begin() +1;

    while (itEnd != resVec.end()) {
        auto e = g.findEdge(*itStart, *itEnd);
        if (e != nullptr) res += e->getDistance();
        itStart++;
        itEnd++;
    }
    auto e = g.findEdge(*itStart, root);
    if (e != nullptr) res += e->getDistance();

    return resVec;
}

void printPath(const vector<int>& resVec, const double& res, const int& rootV, chrono::duration<double> duration) {
    cout << "There is a path with cost " << res << " for this graph:" << endl;

    for (auto node : resVec) {
        cout << " " << node << " ==>";
    }
    cout << " " << rootV << endl;

    cout << "Execution time: " << duration.count() << " seconds." << endl;
}

void printTriangularTSPAproximation(Graph& g) {
    string order;
    Vertex* v = nullptr;
    int rootV = 0;
    while (v == nullptr) v = getNodeInput(g, order, rootV);

    double res = 0;
    vector<int> resVec;

    auto start = chrono::high_resolution_clock::now();

    resVec = triangularAproxiamtion(g, rootV, res);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    if (resVec.size() < g.getNumVertex()) {
        cout << "There is no TSP solution for this graph." << endl;
        return;
    }

    printPath(resVec, res, rootV, duration);
}
