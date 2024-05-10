#include <iostream>
#include "Graph.h"
#include "Backtracking.h"
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

void completeGraph(Graph& g)
{
    unordered_map<int,Vertex*> vertexSet = g.getVertexSet();
    for (pair<int,Vertex*> firstPair : vertexSet)
    {
        if(firstPair.second->getAdj().size()-1 == g.getNumVertex()) continue;
        for(pair<int,Vertex*> secondPair : vertexSet)
        {
            if (firstPair.first != secondPair.first)
            {
                if(g.addEdge(firstPair.first, secondPair.first, 0))//TODO update distance
                {
                    if(firstPair.second->getAdj().size()-1 == g.getNumVertex()) break;
                }
            }
        }
    }
}

void Algorithm_Menu(Graph& g){
    string order;
    while(true){
        cout << endl << "What do you wish to do?" << endl << endl <<
        "Please insert:" << endl <<
        "'1' to execute a backtracking algorithm;" << endl <<
        "'2' to execute a triangular approximation Heuristic algorithm;" << endl <<
        "'3' to execute a different Heuristic algorithm;" << endl <<
        "'4' to resolve the TSP problem using a non-fully connected graph;" << endl <<
        "'close' to exit the program." << endl << endl <<
        "DISCLAIMER: If you choose the option 1 it is advised to use the Toy graphs!" << endl;
        getline(cin >> ws, order);
        if(order == "close"){
            cout << "CLosing now..." << endl;
            return;
        }
        else if(order == "1"){
            cout << tspBT(g) << endl;
        }
        else if(order == "2"){}
        else if(order == "3"){}
        else cout << "Insert a valid number!" << endl;
    }
}
void Graph_Menu(const string &graph_type, Graph& g){
    string option;
    while(option.empty()){
        if(graph_type == "Toy"){
            cout << "Choose one of the available Toy graphs:" << endl <<
             "'1' for the shipping graph." << endl <<
            "'2' for the stadiums graph." << endl <<
            "'3' for the tourism graph." << endl;
            getline(cin >> ws, option);
            if(option == "1") g = buildSimpleGraph("../datasets/Toy-Graphs/Toy-Graphs/shipping.csv");
            else if(option == "2") g = buildSimpleGraph("../datasets/Toy-Graphs/Toy-Graphs/stadiums.csv");
            else if(option == "3") g = buildSimpleGraph("../datasets/Toy-Graphs/Toy-Graphs/tourism.csv");
        }
        else if(graph_type == "Medium-Sized"){
            cout << "Choose one of the available Medium-Sized graphs: (put only the number.For example, '25')" << endl;
            getline(cin >> ws, option);

            string file = "edges_" + option + ".csv";
            g = buildComplexGraph("../datasets/Extra_Fully_Connected_Graphs/Extra_Fully_Connected_Graphs/",file);
        }
        else if(graph_type == "Real-Life"){
            //TODO
        }
    }
    completeGraph(g);
    Algorithm_Menu(g);
}
int main() {
    Graph g;
    string graph_type;
    std::cout << "Welcome!" << std::endl;
    while(graph_type.empty()){
        cout << "Which type of graph do you want to use['Toy'/'Medium-Sized'/'Real-Life' Graph]:" << endl << endl;
        getline(cin >> ws, graph_type);
    }
    Graph_Menu(graph_type, g);

    return 0;
}
