#ifndef _CVRP_H
#define _CVRP_H

#include <vector>
#include <string>
#include <map>

using namespace std;

struct Node{
    double x;
    double y;
    int demand;
};

struct CVRP {
    string name;
    string type;
    int optimal;
    int dimension;
    int numberOfTrucks;
    int capacity;
    int depot;
    vector<Node> nodes;
    vector<vector<int>> routes;
    double lengthOfRoutes;
};

double distanceTo(Node &a, Node &b);
double calculateRouteCost(vector<int> &route, vector<Node> &nodes); // calculate cost of one truck
CVRP readFile(const string &filename);
void printClarkeWrightResult(CVRP &cvrp, string heuristic);
void writeResultToFile(CVRP &cvrp, const string &filename);
#endif
