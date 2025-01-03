#include "CVRP.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include<regex>
#include<cmath>

using namespace std;

double distanceTo(Node &a, Node &b){
    double dist = sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
    return dist;
}

double calculateRouteCost(vector<int> &route, vector<Node> &nodes){
    double cost = 0;
    for (int i = 1; i < route.size(); i++){
        cost += distanceTo(nodes[route[i - 1]], nodes[route[i]]);
    }
    return cost;
}

CVRP readFile(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open file " << filename << endl;
        exit(1);
    }

    CVRP cvrp;
    string line;

    cout << "Reading file: " << filename << "\n";

    while (getline(file, line)) {
        if (line.empty()) continue;
        istringstream iss(line);
        string key;
        char c;
        iss >> key;
        if (key == "NAME") {
            iss >> c >> cvrp.name;
        }
        else if (key == "COMMENT") {
            string comment;
            getline(iss, comment);
            regex rgx("No of trucks: (\\d+), Optimal value: (\\d+)");
            smatch match;
            if (regex_search(comment, match, rgx)) {
                cvrp.numberOfTrucks = stoi(match[1].str());
                cvrp.optimal = stoi(match[2].str());
            }
        }
        else if (key == "TYPE") {
            iss >> cvrp.type;
        }
        else if (key == "DIMENSION") {
            iss >> c >> cvrp.dimension;
            cvrp.nodes.resize(cvrp.dimension + 1);
        }
        else if (key == "CAPACITY") {
            iss >> c >> cvrp.capacity;
        }
        else if (key == "NODE_COORD_SECTION") {
            for (int i = 1; i <= cvrp.dimension; i++) {
                int id;
                double x, y;
                file >> id >> x >> y;
                Node node = {x, y, 0};
                cvrp.nodes[id] = node;
            }
        }
        else if (key == "DEMAND_SECTION") {
            for (int i = 1; i <= cvrp.dimension; i++) {
                int id, demand;
                file >> id >> demand;
                cvrp.nodes[id].demand = demand;
            }
        }
        else if (key == "DEPOT_SECTION") {
            int depot;
            file >> depot;
            cvrp.depot = depot;
        }
    }

    file.close();
    return cvrp;
}

void writeResultToFile(CVRP &cvrp, const string &filename){
    ofstream outfile(filename);
    if (!outfile){
        cerr << "Error";
        return;
    }
    outfile << "Name: " << cvrp.name << endl;
    outfile << "Optimal cost: " << cvrp.optimal << endl;
    outfile << "Total cost: " << cvrp.lengthOfRoutes << endl;
    outfile << "Routes: " << endl;
    for (int i = 0; i < cvrp.routes.size(); i++){
        outfile << "Route " << i + 1 <<  ": ";
        for (int j = 0; j < cvrp.routes[i].size(); j++){
            outfile << cvrp.routes[i][j];
            if (j < cvrp.routes[i].size() - 1){
                outfile << " -> ";
            }
        }
        outfile << endl;
    }
    outfile << "Coordinates of routes: " << endl;
    for (int i = 0; i < cvrp.routes.size(); i++){
        outfile << "Route " << i + 1 << ": ";
        for (int j = 0; j < cvrp.routes[i].size(); j++){
            int node = cvrp.routes[i][j];
            outfile << "(" << cvrp.nodes[node].x << ", " << cvrp.nodes[node].y << ")";
            if (j < cvrp.routes[i].size() - 1) outfile << " -> ";
        }
        outfile << endl;
    }

    outfile.close();
}

void printClarkeWrightResult(CVRP &cvrp, string heuristic){
    cout << "FILE: " << cvrp.name << endl;
    cout << "Heuristic: " << heuristic << endl;
    cout << "Optimal cost: " << cvrp.optimal << endl;
    cout << "Total cost: " << cvrp.lengthOfRoutes << endl;
    cout << "Route of each truck: " << endl;
    for (int i = 0; i < cvrp.routes.size(); i++){
        cout << "Route " << i + 1 << ": ";
        for (int j = 0; j < cvrp.routes[i].size(); j++) {
            cout << cvrp.routes[i][j];
            if (j < cvrp.routes[i].size() - 1) std::cout << " -> ";
        }
        cout << "\n";
    }
}
