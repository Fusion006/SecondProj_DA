#include "common.h"


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

void printPath(const vector<int>& resVec, const double& res, const int& rootV, chrono::duration<double> duration) {
    cout << "There is a path with cost " << res << " for this graph:" << endl;

    for (auto node : resVec) {
        cout << " " << node << " ==>";
    }
    cout << " " << rootV << endl;

    cout << "Execution time: " << duration.count() << " seconds." << endl;
}