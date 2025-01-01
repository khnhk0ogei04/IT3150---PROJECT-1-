#include "TwoOpt.h"
#include "CVRP.h"
#include <algorithm>
#include <iostream>
#include <cmath>

using namespace std;
void optimizeTwoOptRoute(vector<int> &route, vector<Coord> &nodes) {
    bool improved = true;

    while (improved) {
        improved = false;
        for (int i = 1; i < route.size() - 2; i++) {
            for (int j = i + 1; j < route.size() - 1; j++) {
                double oldDistance = distanceTo(nodes[route[i - 1]], nodes[route[i]]) +
                                     distanceTo(nodes[route[j]], nodes[route[j + 1]]);
                double newDistance = distanceTo(nodes[route[i - 1]], nodes[route[j]]) +
                                     distanceTo(nodes[route[i]], nodes[route[j + 1]]);

                if (newDistance < oldDistance) {
                    reverse(route.begin() + i, route.begin() + j + 1);
                    improved = true;
                }
            }
        }
    }
}

void runTwoOptFromClarkeWright(CVRP &cvrp) {
    double initialCost = 0, optimizedCost = 0;

    for (auto &route : cvrp.routes) {
        initialCost += calculateRouteCost(route, cvrp.nodes);
    }

    for (auto &route : cvrp.routes) {
        optimizeTwoOptRoute(route, cvrp.nodes);
    }

    for (auto &route : cvrp.routes) {
        optimizedCost += calculateRouteCost(route, cvrp.nodes);
    }

    cvrp.lengthOfRoutes = optimizedCost;

    printClarkeWrightResult(cvrp, "TwoOpt");
}
