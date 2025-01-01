#ifndef _CVRP_H
#define _CVRP_H

#include <vector>
#include <string>
#include <map>

using namespace std;

struct Coord{
    double x;
    double y;
};

struct CVRP {
    string name;
    string type;
    int optimal;
    int dimension;
    int numberOfTrucks;
    int capacity;
    int depot;
    vector<Coord> nodes;
    vector<vector<int>> routes;
    vector<int> demands;
    double lengthOfRoutes;
};

double distanceTo(Coord &a, Coord &b);
double calculateRouteCost(vector<int> &route, vector<Coord> &nodes); // calculate cost of one truck
CVRP readFile(const string &filename);
void printClarkeWrightResult(CVRP &cvrp, string heuristic);
void writeResultToFile(CVRP &cvrp, const string &filename);
#endif
