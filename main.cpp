#include <iostream>
#include "Graph.h"
#include "haversine.h"
using namespace std;




Graph buildSimpleGraph(const string& filepath){
    Graph g;
    ifstream ifile(filepath);
    string line;
    if (ifile.is_open())
    {
        getline(ifile,line);
        while (getline(ifile,line))
        {
            istringstream stringline(line);
            string pointA; getline(stringline,pointA,',');
            string pointB; getline(stringline,pointB,',');
            string distance; getline(stringline,distance,',');

            g.addVertex(stoi(pointA),pointA);
            g.addVertex(stoi(pointB),pointB);
            if (!g.addEdge(stoi(pointA), stoi(pointB), stod(distance)))
            {
                cout << "Error in reading simple Graph couldn't add edge";
                exit(EXIT_FAILURE);
            }
        }
    }else{
        cout << "Error in reading simple Graph file not found";
        exit(EXIT_FAILURE);
    }
    return g;
}

Graph buildComplexGraph(const string& dirpath, const string& filename)
{
    Graph g;
    ifstream edgesFile(dirpath+filename);
    string line;
    if (edgesFile.is_open())
    {
        getline(edgesFile,line);
        while (getline(edgesFile,line))
        {
            istringstream stringline(line);
            string pointA; getline(stringline,pointA,',');
            string pointB; getline(stringline,pointB,',');
            string distance; getline(stringline,distance,',');

            g.addVertex(stoi(pointA),pointA);
            g.addVertex(stoi(pointB),pointB);
            if (!g.addEdge(stoi(pointA), stoi(pointB), stod(distance)))
            {
                cout << "Error in reading complex Graph couldn't add edge";
                exit(EXIT_FAILURE);
            }
        }
    }else{
        cout << "Error in reading complex Graph file not found";
        exit(EXIT_FAILURE);
    }



    ifstream nodesFile(dirpath+"nodes.csv");
    if (nodesFile.is_open())
    {
        getline(nodesFile,line);
        while (getline(nodesFile,line))
        {
            istringstream stringline(line);
            string point; getline(stringline,point,',');
            string lat; getline(stringline,lat,',');
            string lon; getline(stringline,lon,',');

            Vertex* vertex = g.findVertex(stoi(point));
            if (vertex != nullptr)
            {
                vertex->setLat(stod(lat));
                vertex->setLon(stod(lon));
            }

        }
    }else{
        cout << "Error in reading complex Graph file not found";
        exit(EXIT_FAILURE);
    }
    return g;
}

void completeGraph(const Graph& g)
{
    unordered_map<int,Vertex*> vertexSet = g.getVertexSet();
    for (pair<int,Vertex*> firstPair : vertexSet)
    {
        if(firstPair.second->getAdj().size()-1 == g.getNumVertex()) continue;
        for(pair<int,Vertex*> secondPair : vertexSet)
        {
            if (firstPair.first != secondPair.first)
            {
                double dist = haversine(firstPair.second->getLat(), firstPair.second->getLon(),
                                        secondPair.second->getLat(), secondPair.second->getLon());
                if(g.addEdge(firstPair.first, secondPair.first, dist))
                {
                    if(firstPair.second->getAdj().size()-1 == g.getNumVertex()) break;
                }
            }
        }
    }
}

int main() {

    //Graph g = buildSimpleGraph("../datasets/Toy-Graphs/Toy-Graphs/shipping.csv");
    Graph g = buildComplexGraph("../datasets/Extra_Fully_Connected_Graphs/Extra_Fully_Connected_Graphs/","edges_25.csv");
    completeGraph(g);


    cout<<"Hello world";
    return 0;
}
