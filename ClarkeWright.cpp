#include "ClarkeWright.h"
#include <algorithm>
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

const int VRPH_UNUSED = 0;
const int VRPH_ADDED = 1;
const int VRPH_INTERIOR = 2;

bool compareSavingCost(Saving &A, Saving &B) {
    return A.savingCost > B.savingCost;
}

void recalculateCapacities(vector<vector<int>> &routes, vector<int> &capacities, const vector<Node> &nodes) {
    capacities.clear();
    for (const auto &route : routes) {
        int totalDemand = 0;
        for (int i = 1; i < route.size() - 1; ++i) {
            totalDemand += nodes[route[i]].demand;
        }
        capacities.push_back(totalDemand);
    }
}


void runClarkeWright(CVRP &cvrp) {
    int nodeCount = cvrp.dimension;
    int depot = cvrp.depot;
    int capacity = cvrp.capacity;

    // initialize routes
    vector<vector<int>> routes;
    vector<int> capacities;
    vector<int> status(nodeCount + 1, VRPH_UNUSED);

    for (int i = 2; i <= nodeCount; i++) {
        routes.push_back({depot, i, depot});
        capacities.push_back(cvrp.nodes[i].demand);
    }

    // calculate saving matrix
    vector<Saving> savingList;
    vector<vector<double>> dist(nodeCount + 1, vector<double>(nodeCount + 1, 0.0));
    for (int i = 1; i <= nodeCount; i++) {
        for (int j = 1; j <= nodeCount; j++) {
            dist[i][j] = distanceTo(cvrp.nodes[i], cvrp.nodes[j]);
        }
    }

    for (int i = 2; i <= nodeCount - 1; i++) {
        for (int j = i + 1; j <= nodeCount; j++) {
            double savingVal = dist[depot][i] + dist[depot][j] - dist[i][j];
            savingList.push_back({i, j, savingVal});
        }
    }
    sort(savingList.begin(), savingList.end(), compareSavingCost);
    for (Saving s : savingList) {
        int i = s.startNode, j = s.endNode;
        int routeI = -1, routeJ = -1;
        // find route of i, j
        for (int r = 0; r < routes.size(); r++) {
            if (find(routes[r].begin(), routes[r].end(), i) != routes[r].end()) {
                routeI = r;
            }
            if (find(routes[r].begin(), routes[r].end(), j) != routes[r].end()) {
                routeJ = r;
            }
        }
        if (status[i] == VRPH_INTERIOR || status[j] == VRPH_INTERIOR){
            continue;
        }
        if (routeI == routeJ) continue;
    //for (auto r: routes){
    //        for (int node : r){
    //            cout << node << " ";
    //        }
    //        cout <<endl;
    //    }
        if (status[i] == VRPH_UNUSED && status[j] == VRPH_UNUSED){
            if (cvrp.nodes[i].demand + cvrp.nodes[j].demand <= capacity){
                routes[routeI].insert(routes[routeI].end() - 1, j);
                status[i] = VRPH_ADDED;
                status[j] = VRPH_ADDED;
                routes.erase(routes.begin() + routeJ);
                recalculateCapacities(routes, capacities, cvrp.nodes);
            }
        }
        // case 1: i is added, j is unused
        else if (status[i] == VRPH_ADDED && status[j] == VRPH_UNUSED) {
            if (capacities[routeI] + cvrp.nodes[j].demand <= capacity) {
                if (routes[routeI].front() == depot && routes[routeI][1] == i) {
                    routes[routeI].insert(routes[routeI].begin() + 1, j);
                    routes.erase(routes.begin() + routeJ);
                } else if (routes[routeI][routes[routeI].size() - 2] == i && routes[routeI].back() == depot) {
                    routes[routeI].insert(routes[routeI].end() - 1, j);
                    routes.erase(routes.begin() + routeJ);
                }
                status[j] = VRPH_ADDED;
                status[i] = VRPH_INTERIOR;
                recalculateCapacities(routes, capacities, cvrp.nodes);
            }
        }
        // case 2: i is unused, j is added
        else if (status[i] == VRPH_UNUSED && status[j] == VRPH_ADDED) {
            if (capacities[routeJ] + cvrp.nodes[i].demand <= capacity) {
                if (routes[routeJ].front() == depot && routes[routeJ][1] == j) {
                    routes[routeJ].insert(routes[routeJ].begin() + 1, i);
                    routes.erase(routes.begin() + routeI);
                } else if (routes[routeJ][routes[routeJ].size() - 2] == j && routes[routeJ].back() == depot) {
                    routes[routeJ].insert(routes[routeJ].end() - 1, i);
                    routes.erase(routes.begin() + routeI);
                }
                status[i] = VRPH_ADDED;
                status[j] = VRPH_INTERIOR;
                recalculateCapacities(routes, capacities, cvrp.nodes);
            }
        }
        else if (status[i] == VRPH_ADDED && status[j] == VRPH_ADDED) {
            if (capacities[routeI] + capacities[routeJ] <= capacity) {
                if (routes[routeI][1] == i && routes[routeJ][routes[routeJ].size() - 2] == j) {
                    // i at 2nd, j at -2nd
                    routes[routeI].insert(routes[routeI].begin() + 1, routes[routeJ].begin() + 1, routes[routeJ].end() - 1);
                    status[i] = VRPH_INTERIOR;
                    status[j] = VRPH_INTERIOR;
                    routes.erase(routes.begin() + routeJ);
                     recalculateCapacities(routes, capacities, cvrp.nodes);
                } else if (routes[routeI][routes[routeI].size() - 2] == i && routes[routeJ][1] == j) {
                    // i at -2nd, j at 2nd
                    routes[routeI].insert(routes[routeI].end() - 1, routes[routeJ].begin() + 1, routes[routeJ].end() - 1);
                    status[i] = VRPH_INTERIOR;
                    status[j] = VRPH_INTERIOR;
                    routes.erase(routes.begin() + routeJ);
                    recalculateCapacities(routes, capacities, cvrp.nodes);
                }
            }
        }
    }
    double totalCost = 0.0;
    for (auto &route : routes) {
        double cost = calculateRouteCost(route, cvrp.nodes);
        totalCost += cost;
        cvrp.routes.push_back(route);
    }

    cvrp.lengthOfRoutes = totalCost;
    printClarkeWrightResult(cvrp, "Clarke-Wright-Saving");
}
