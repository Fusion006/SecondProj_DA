#include <iostream>
#include <algorithm>
#include "Graph.h"
#include "Backtracking.h"
#include "haversine.h"
#include "heuristic/antColOpt.h"
#include "heuristic/christofides.h"

#include "heuristic/Triangular.h"

using namespace std;
/**
 * Creates a copy for a graph.
 * gCopy turns into a copy of @gOrigin.
 * @param gOrigin graph that is about about to be copied.
 * @param gCopy graph containing a copy of the graph @gOrigin.
 */
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
/**
 * Function that converts a data file into a graph with weighted edges.
 * Creates a simple graph, so this function is only to be used for Toy Graphs.
 * @param filepath path of the file that contains all the information about the edges.
 * @return the Toy graph according to choices of the user.
 */
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

            double dist = stod(distance);
            //int dista = static_cast<int>(round(dist * 10));
            if (!g.addEdge(stoi(pointA), stoi(pointB), dist))
            {
                cout << "Error in reading simple Graph couldn't add edge";
                exit(EXIT_FAILURE);
            }
            if (!g.addEdge(stoi(pointB), stoi(pointA), dist))
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

/**
 * Function that converts a data file into a graph with weighted edges.
 * Creates a complex graph, so not only are the edges weighted, but the vertexs also have coordinates.
 * @param dirpath path of the directory that contains the file with all the information about the edges.
 * @param filename name of the file with the information about all the edges.
 * @param numOfNodes number of nodes that will be processed and used to create the graph.
 * @return the Medium/Real-Life graph according to choices of the user.
 */
Graph buildComplexGraph(const string& dirpath, const string& filename, const int& numOfNodes)
{
    Graph g;
    ifstream edgesFile(dirpath+filename);
    string line;
    if (edgesFile.is_open())
    {
        getline(edgesFile,line);
        while (getline(edgesFile,line)) {
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
            if (!g.addEdge(stoi(pointB), stoi(pointA), stod(distance)))
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

/**
 * Function that completes any graph.
 * Turns a graph into a fully connected graph.
 * @param g graph that will receive the new nodes.
 */
void completeComplexGraph(Graph& g)
{
    unordered_map<int,Vertex*> vertexSet = g.getVertexSet();
    for (pair<int,Vertex*> firstPair : vertexSet)
    {
        if(firstPair.second->getAdj().size() == g.getNumVertex() -1) continue;
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

/** Function that takes care of the User Interface to chose the algorithm he desires.
 *  This function receives the user's orders and answers them according to the user's wish.
 * @param g graph to give as an argument to the functions that actually respond to the user tasks.
 * @param gCompleted fully connected copy of the graph g to use if needed.
 */
void Run(Graph& g){
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
        else if(order == "1") {printBacktrackingSolution(g);}

        else if(order == "2") {printTriangularTSPAproximation(g);}

        else if(order == "3"){
            buildMST(g);
            christofides(g);

        }

        else if(order == "4"){

        }

        else cout << "Insert a valid number!" << endl;
    }
}

/** Function that takes care of the User Interface to choose the graph he desires.
 *  This function receives the user's orders and answers them according to the user's wish.7
 *  @param graph_type type of graph dataset chosen by the user.
 * @param g graph that is about to be chosen by the user.
 * @param gCompleted fully connected copy of the graph g to use if needed.
 */
void Graph_Menu(const string &graph_type, Graph& g){
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
            completeComplexGraph(g);
        }

        else if(graph_type == "Real"){
            option = "";
            //TODO
        }
    }
}

int main() {
    Graph g;
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

    Graph_Menu(graph_type, g);
    Run(g);

    return 0;
}
