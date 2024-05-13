#include <iostream>
#include <algorithm>
#include "Graph.h"
#include "Backtracking.h"
#include "haversine.h"
using namespace std;

void graphCopy(Graph& gOrigin, Graph& gCopy) {

    for (pair<const int, Vertex *> pair : gOrigin.getVertexSet()) {
        gCopy.addVertex(pair.second->getId(), pair.second->getName(), pair.second->getLat(), pair.second->getLon());
    }

    for (auto pair : gOrigin.getVertexSet()) {
        for (auto pairEdge : pair.second->getAdj()) {
            if(gCopy.addEdge(pairEdge.second->getOrig()->getId(), pairEdge.second->getDest()->getId(), pairEdge.second->getDistance())) continue;
        }
    }
}

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
            if (!g.addEdge(stoi(pointB), stoi(pointA), stod(distance)))
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

Graph buildComplexGraph(const string& dirpath, const string& filename, const int& numOfNodes)
{
    Graph g;
    ifstream edgesFile(dirpath+filename);
    string line;
    if (edgesFile.is_open())
    {
        getline(edgesFile,line);
        //TODO Apenas ler o numero de nodes pedido
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

void completeComplexGraph(Graph& g)
{
    unordered_map<int,Vertex*> vertexSet = g.getVertexSet();
    for (pair<int,Vertex*> firstPair : vertexSet)
    {
        if(firstPair.second->getAdj().size()-1 == g.getNumVertex()) continue;
        for(pair<int,Vertex*> secondPair : vertexSet)
        {
            if (firstPair.first != secondPair.first)
            {
                double dist = haversine(firstPair.second->getLat(), firstPair.second->getLon(), secondPair.second->getLat(), secondPair.second->getLon());
                if(g.addEdge(firstPair.first, secondPair.first, dist))
                {
                    if(firstPair.second->getAdj().size()-1 == g.getNumVertex()) break;
                }
            }
        }
    }
}

void Run(Graph& g, Graph& gCompleted){
    string order;
    while(true){
        cout << endl << "What do you wish to do?" << endl << endl <<
             "Please insert:" << endl <<
             "'1' to execute a backtracking algorithm;" << endl <<
             "'2' to execute a triangular approximation Heuristic algorithm;" << endl <<
             "'3' to execute our Heuristic algorithm;" << endl <<
             "'4' to execute our Heuristic algorithm for non-fully connected graphs;" << endl <<
             "'close' to exit the program." << endl << endl <<
             "DISCLAIMER: If you choose the option 1 it is advised to use only the Toy graphs!" << endl;

        getline(cin >> ws, order);

        if(order == "close"){
            cout << "CLosing now..." << endl;
            return;
        }

        else if(order == "1"){
            printBacktrackingSolution(g);
        }

        else if(order == "2"){

        }

        else if(order == "3"){

        }

        else if(order == "4"){

        }

        else cout << "Insert a valid number!" << endl;
    }
}
void Graph_Menu(const string &graph_type, Graph& g, Graph& gCompleted){
    string option;
    while(option.empty()){

        if(graph_type == "Toy"){
            cout << "Choose one of the available Toy graphs[shipping/stadiums/tourism]:" << endl;

            getline(cin >> ws, option);

            if(option == "shipping") g = buildSimpleGraph("../datasets/Toy-Graphs/Toy-Graphs/shipping.csv");

            else if(option == "stadiums") g = buildSimpleGraph("../datasets/Toy-Graphs/Toy-Graphs/stadiums.csv");

            else if(option == "tourism") g = buildSimpleGraph("../datasets/Toy-Graphs/Toy-Graphs/tourism.csv");

            else {
                cout << "Insert a valid graph!" << endl;
                option = "";
                continue;
            }
            graphCopy(g, gCompleted);
            return;
        }

        else if(graph_type == "Medium"){
            vector<string> numOfNodes = {"25", "50", "75", "100", "200", "300", "400", "500", "600", "700", "800", "900"};
            cout << "Choose the number of nodes[25/50/75/100/200/300/400/500/600/700/800/900]:" << endl;
            getline(cin >> ws, option);

            if (find(numOfNodes.begin(), numOfNodes.end(), option) == numOfNodes.end()) {
                cout << "Insert a valid number!" << endl;
                option = "";
                continue;
            }

            string file = "edges_" + option + ".csv";
            int n = stoi(option);
            g = buildComplexGraph("../datasets/Extra_Fully_Connected_Graphs/Extra_Fully_Connected_Graphs/",file, n);
            graphCopy(g, gCompleted);
            completeComplexGraph(gCompleted);
        }

        else if(graph_type == "Real"){
            option = "";
            //TODO
        }
    }
}

int main() {
    Graph g, gCompleted;
    string graph_type;
    std::cout << "Welcome!" << std::endl;

    while(graph_type.empty()){
        cout << "Which dataset do you want to use[Toy/Medium/Real]:" << endl;

        getline(cin >> ws, graph_type);

        if (graph_type != "Toy" && graph_type != "Medium" && graph_type != "Real") {
            cout << "Insert a valid dataset!" << endl;
            graph_type = "";
        }
    }

    Graph_Menu(graph_type, g, gCompleted);
    Run(g, gCompleted);

    return 0;
}
