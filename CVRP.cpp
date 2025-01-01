#include "CVRP.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include<math.h>

using namespace std;

double distanceTo(Coord &a, Coord &b){
    double dist = sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
    return dist;
}

double calculateRouteCost(vector<int> &route, vector<Coord> &nodes){
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
        } else if (key == "DIMENSION") {
            iss >> c >> cvrp.dimension;
            cvrp.nodes.resize(cvrp.dimension + 1);
            cvrp.demands.resize(cvrp.dimension + 1, 0);
        } else if (key == "CAPACITY") {
            iss >> c >> cvrp.capacity;
        } else if (key == "NODE_COORD_SECTION") {
            for (int i = 1; i <= cvrp.dimension; i++) {
                int id;
                double x, y;
                file >> id >> x >> y;
                Coord coord = {x, y};
                cvrp.nodes[id] = coord;
            }
        } else if (key == "DEMAND_SECTION") {
            for (int i = 1; i <= cvrp.dimension; i++) {
                int id, demand;
                file >> id >> demand;
                cvrp.demands[id] = demand;
            }
        } else if (key == "DEPOT_SECTION") {
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
