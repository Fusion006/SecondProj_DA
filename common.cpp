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
    cout << "There is a path with a cost approximately of " << res << " for this graph:" << endl;

    for (auto node : resVec) {
        cout << " " << node << " ==>";
    }
    cout << " " << rootV << endl;

    cout << "Execution time: " << duration.count() << " seconds." << endl;
}

void dfs_scc(Graph&g, Vertex *v, stack<int> &s, vector<set<int>> &l, int &i) {
    v->setVisited(true);
    v->setLow(i);
    v->setNum(i);
    v->setProcessing(true);
    s.push(v->getId());
    i++;
    for (auto & e : v->getAdj()) {
        auto w = e.second->getDest();
        if (!w->isVisited()) {
            dfs_scc(g, w, s, l, i);
            v->setLow(min(v->getLow(), w->getLow()));
        } else if (w->isProcessing())
            v->setLow(min(v->getLow(), w->getNum()));
    }
    if (v->getNum() == v->getLow()) {
        Vertex* w;
        set<int> scc;
        do {
            w = g.findVertex(s.top());
            w->setProcessing(false);
            scc.insert(w->getId());
            s.pop();
        } while (w != v);
        l.push_back(scc);
    }
}

vector<set<int>> sccTarjan(Graph& g) {
    int index = 1;
    stack<int> s;
    vector<set<int>> res;
    for(auto j : g.getVertexSet()){
        j.second->setVisited(false);
    }
    for(auto i : g.getVertexSet()){
        if(!i.second->isVisited()){
            dfs_scc(g, i.second,s, res, index);
        }
    }
    return res;
}

bool checkIfFullyConnected(Graph& g) {
    auto l = sccTarjan(g);
    if (l.size() > 1) return false;
    return true;
}
