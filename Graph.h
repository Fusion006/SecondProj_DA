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



    void setVisited(bool state);
    void setPath(Edge* newPath);
    Edge* addPipe(Vertex *dest, double distance);
private:
    int id;
    string name;
    double lat;
    double lon;
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

    void setSelected(bool state);
    void setReverse(Edge* reverse);

protected:
    Vertex* dest; // destination vertex
    double distance; // edge weight, can also be used for distance
    bool selected = false;

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
    [[nodiscard]] inline Edge *findPipe(const int &source, const int &target) const;

    bool addVertex(int id, const string &name, double latitude = INVALID_COORDINATE, double longitude = INVALID_COORDINATE);

    /*
     * Adds an edge to a graph (this), given the contents of the source and
     * destination vertices and the edge weight (w).
     * Returns true if successful, and false if the source or destination vertex does not exist.
     */
    [[nodiscard]] bool addEdge(const int &sourceid, const int &destId, double dist) const;

    [[nodiscard]] inline size_t getNumVertex() const;
    [[nodiscard]] inline unordered_map<int,Vertex*> getVertexSet() const;

protected:
    std::unordered_map<int,Vertex*> vertexSet;    // vertex set


public:
    map<string,int> nameToCode;

};

/************************* Vertex  **************************/

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
inline Edge* Vertex::addPipe(Vertex *dest, double distance) {
    auto newEdge = new Edge(this, dest, distance);
    adj[dest->id] = newEdge;
    return newEdge;
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

inline void Edge::setSelected(bool state) {
    this->selected = state;
}
inline void Edge::setReverse(Edge *reversePipe) {
    this->reverse = reversePipe;
}

/********************** Graph  ****************************/


size_t Graph::getNumVertex() const {
    return vertexSet.size();
}
unordered_map<int,Vertex*> Graph::getVertexSet() const {
    return vertexSet;
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
    if (v1 == nullptr || v2 == nullptr)
        return false;
    v1->addPipe(v2, dist);
    return true;
}



inline bool Graph::addVertex(int id, const string &name, double latitude, double longitude) {
    
    if(findVertex(id) != nullptr) return false;
    auto* newVertex = new Vertex(id, name,latitude,longitude);
    vertexSet[id] = newVertex;
    return true;
}

Edge *Graph::findPipe(const int &source, const int &target) const {
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
