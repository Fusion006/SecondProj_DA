#ifndef DAP2_GRAPH_H
#define DAP2_GRAPH_H

#include <fstream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <queue>
#include <map>
#include <unordered_map>
#define INVALID_COORDINATE 200
using namespace std;
class Edge;

/************************* Vertex  **************************/
/**
 * @class GenericVertex
 * @brief Generic function that represents a Graphs Vertex
 */
class Vertex {
public:

    Vertex(int id, string code, double latitude = INVALID_COORDINATE, double longitude = INVALID_COORDINATE){
        this->id = id;
        this->name = std::move(code);
        this->lat = latitude;
        this->lon = longitude;
    }

    [[nodiscard]] inline unordered_map<int,Edge *> getAdj() const;
    [[nodiscard]] inline bool isVisited() const;
    [[nodiscard]] int getId() const;
    [[nodiscard]] string getName() const;
    [[nodiscard]] double getLat() const;
    [[nodiscard]] double getLon() const;
    [[nodiscard]] Edge* getPath() const;
    [[nodiscard]] int getNum() const;
    [[nodiscard]] int getLow() const;
    [[nodiscard]] bool isProcessing() const;

    void setProcessing(bool p);
    void setVisited(bool state);
    void setPath(Edge* newPath);
    void setLat(double newLat);
    void setLon(double newLon);
    void setLow(int low);
    void setNum(int num);
    Edge* addEdge(Vertex *dest, double distance);
    Edge* addCopyEdge(Edge* copiedEdge);
    void eraseCopyEdges();

private:
    int id;
    string name;
    double lat;
    double lon;
    bool processing;       // auxiliary field
    int num;               // auxiliary field
    int low;               // auxiliary field
    unordered_map<int,Edge*> adj;
    Edge* path = nullptr;

    bool visited = false;
};

/********************** Edge  ****************************/
/**
 * @class Edges
 * @Brief A representation of the Graphs edges
 */
class Edge {
public:
    Edge(Vertex* orig, Vertex* dest, double w);

    [[nodiscard]] inline Vertex* getDest() const;
    [[nodiscard]] inline double getDistance() const;
    [[nodiscard]] inline Vertex* getOrig() const;
    [[nodiscard]] inline Edge* getReverse() const;
    [[nodiscard]] inline bool getSelected() const;
    [[nodiscard]] inline bool isUsed() const;
    [[nodiscard]] inline double getPheromones() const;
    [[nodiscard]] inline double getTransitionProbability() const;
    inline bool operator<(Edge e2) const{
        if (this->distance < e2.distance) return false;
        if (this->distance > e2.distance) return true;
        if (this->orig->getId() < e2.dest->getId()) return false;
        return this->orig->getId() > e2.dest->getId();


    }

    void setSelected(bool state);
    void setUsed(bool state);
    void setReverse(Edge* reverse);
    void setPheromones(double newPheromones);
    void setTransitionProbability(double newProb);


    void evaporatePheromones(double persistenceRate);

protected:
    Vertex* dest; // destination vertex
    double distance; // edge weight, can also be used for distance
    bool selected = false;
    bool used = false; //useful if we only want to use 'selected' edges but keep track of which edges have already been used
    double pheromones = 1;
    double transitionProbability = 0;

    Vertex *orig;
    Edge *reverse = nullptr;

};

/********************** Graph  ****************************/
/**
 * @class Graph
 * @brief Class that represents the dataset as a graph
 */
class Graph {
public:
    Graph();
    [[nodiscard]] inline Vertex *findVertex(const int &id) const;
    [[nodiscard]] inline Edge *findEdge(const int &source, const int &target) const;

    bool addVertex(int id, const string &name, double latitude = INVALID_COORDINATE, double longitude = INVALID_COORDINATE);

    /*
     * Adds an edge to a graph (this), given the contents of the source and
     * destination vertices and the edge weight (w).
     * Returns true if successful, and false if the source or destination vertex does not exist.
     */
    [[nodiscard]] bool addEdge(const int &sourceid, const int &destId, double dist) const;

    [[nodiscard]] inline size_t getNumVertex() const;
    [[nodiscard]] inline unordered_map<int,Vertex*> getVertexSet() const;
    inline void cleanGraph();
    inline void eraseCopyEdges();
    inline double getPheromoneDropoff();
    inline void evaporatePheromones(double persistenceRate);


protected:
    std::unordered_map<int,Vertex*> vertexSet;    // vertex set


public:
    unordered_map<string,int> nameToCode;

};

/************************* Vertex  **************************/

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
inline Edge* Vertex::addEdge(Vertex *dest, double distance) {
    auto newEdge = new Edge(this, dest, distance);
    adj[dest->id] = newEdge;

    unordered_map<int,Edge*> adjs = dest->getAdj();
    auto reverseEdge = adjs.find(this->id);
    if ( reverseEdge != dest->getAdj().end() ) //if inverted edge exists
    {
        reverseEdge->second->setReverse(newEdge);
        newEdge->setReverse(reverseEdge->second);
    }
    return newEdge;
}
inline Edge* Vertex::addCopyEdge(Edge* copiedEdge) {
    Vertex* dest = copiedEdge->getDest();
    Edge* copyEdge = new Edge(copiedEdge->getOrig(), dest, copiedEdge->getDistance());
    Edge* reverseCopyEdge = new Edge(dest, copiedEdge->getOrig(), copiedEdge->getDistance());

    adj[(dest->id + 1) * -1]        = copyEdge;
    dest->adj[(this->id + 1) * -1]  = reverseCopyEdge;

    copyEdge->setReverse(reverseCopyEdge);
    reverseCopyEdge->setReverse(copyEdge);

    return copyEdge;
}

inline void Vertex::eraseCopyEdges() {
    for (auto it = this->adj.begin(); it != this->adj.end();)
    {
        if (it->first < 0){
            it = this->adj.erase(it);
        }else{
            it++;
        }
    }
}


inline string Vertex::getName() const {
    return this->name;
}

inline double Vertex::getLat() const {
    return this->lat;
}

inline double Vertex::getLon() const {
    return this->lon;
}

inline int Vertex::getId() const {
    return this->id;
}

unordered_map<int,Edge*> Vertex::getAdj() const {
    return this->adj;
}

bool Vertex::isVisited() const {
    return this->visited;
}


inline Edge* Vertex::getPath() const {
    return this->path;
}

inline void Vertex::setVisited(bool state) {
    this->visited = state;
}

inline void Vertex::setPath(Edge* newPath) {
    this->path = newPath;
}

inline void Vertex::setLat(double newLat)
{
    this->lat=newLat;
}

inline void Vertex::setLon(double newLon) {
    this->lon=newLon;
}

inline void Vertex::setNum(int num) {
    this->num = num;
}

inline void Vertex::setLow(int low) {
    this->low = low;
}

inline int Vertex::getNum() const {
    return num;
}

inline int Vertex::getLow() const {
    return low;
}

inline void Vertex::setProcessing(bool p) {
    this->processing = p;
}
inline bool Vertex::isProcessing() const {
    return processing;
}

/********************** Edge  ****************************/
inline Edge::Edge(Vertex *orig, Vertex *dest, double w): orig(orig), dest(dest), distance(w) {}
Vertex * Edge::getDest() const {
    return this->dest;
}
double Edge::getDistance() const {
    return this->distance;
}
Vertex * Edge::getOrig() const {
    return this->orig;
}
Edge *Edge::getReverse() const {
    return this->reverse;
}
bool Edge::getSelected() const {
    return this->selected;
}
bool Edge::isUsed() const {
    return this->used;
}
double Edge::getPheromones() const {
    return this->pheromones;
}
double Edge::getTransitionProbability() const {
    return this->transitionProbability;
}



inline void Edge::setSelected(bool state) {
    this->selected = state;
}
inline void Edge::setUsed(bool state) {
    this->used = state;
}
inline void Edge::setReverse(Edge *reverseEdge) {
    this->reverse = reverseEdge;
}

inline void Edge::setPheromones(double  newPheromones) {
    this->pheromones = newPheromones;
}

inline void Edge::evaporatePheromones(double persistenceRate){
    this->pheromones *= persistenceRate;
}

inline void Edge::setTransitionProbability(double newProb) {
    this->transitionProbability = newProb;
}


/********************** Graph  ****************************/


size_t Graph::getNumVertex() const {
    return vertexSet.size();
}
unordered_map<int,Vertex*> Graph::getVertexSet() const {
    return vertexSet;
}

void Graph::cleanGraph() {
    for (pair<int,Vertex*> vertex : this->vertexSet)
    {
        vertex.second->setVisited(false);
        vertex.second->setPath(nullptr);
        unordered_map<int,Edge*> edges = vertex.second->getAdj();
        vertex.second->setNum(-1);
        for (pair<int,Edge*> edge : edges)
        {
            edge.second->setPheromones(1);
            edge.second->setSelected(false);
            edge.second->setUsed(false);
        }
    }
}

void Graph::eraseCopyEdges(){
    for (pair<int,Vertex*> vertex : this->vertexSet) {
        vertex.second->eraseCopyEdges();
    }
}

double Graph::getPheromoneDropoff()
{
    double res = 0;
    for (pair<int,Vertex*> vertex : this->vertexSet)
    {
        for (pair<int,Edge*> edge : vertex.second->getAdj())
        {
            res += edge.second->getDistance();
        }
    }
    return res/vertexSet.size();
}


void Graph::evaporatePheromones(double persistenceRate) {
    for (pair<int,Vertex*> vertex : this->vertexSet)
    {
        unordered_map<int,Edge*> edges = vertex.second->getAdj();
        for (pair<int,Edge*> edge : edges)
        {
            edge.second->evaporatePheromones(persistenceRate);
        }
    }
}

/*
 * Auxiliary function to find a vertex with a given content.
 */

Vertex * Graph::findVertex(const int &id) const {
    auto vertex = vertexSet.find(id);
    if (vertexSet.find(id) != vertexSet.end()) return vertex->second;
    return nullptr;
}
/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge distance.
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
inline bool Graph::addEdge(const int &sourceid, const int &destid, double dist) const {
    auto v1 = findVertex(sourceid);
    auto v2 = findVertex(destid);
    if (v1 == nullptr || v2 == nullptr || v1 == v2)
        return false;
    v1->addEdge(v2, dist);
    return true;
}



inline bool Graph::addVertex(int id, const string &name, double latitude, double longitude) {
    
    if(findVertex(id) != nullptr) return false;
    auto* newVertex = new Vertex(id, name,latitude,longitude);
    vertexSet[id] = newVertex;
    nameToCode[name] = id;
    return true;
}

Edge *Graph::findEdge(const int &source, const int &target) const {
    auto vsource = findVertex(source);
    if (vsource == nullptr) {
        return nullptr;
    }
    unordered_map<int, Edge*> adjs = vsource->getAdj();
    auto edge = adjs.find(target);
    if (edge != adjs.end())
    {
        return edge->second;
    }
    return nullptr;
}
#endif
