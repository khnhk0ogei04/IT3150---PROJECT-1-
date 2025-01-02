#include "ClarkeWright.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

bool compareSavingCost(const Saving &A, const Saving &B) {
    if (A.savingCost == B.savingCost){
        return A.startNode > B.startNode;
    }
    return A.savingCost > B.savingCost;
}

void runClarkeWright(CVRP &cvrp)
{
    int nodeCount     = cvrp.dimension;
    int depot         = cvrp.depot;
    int capacity      = cvrp.capacity;
    int truckCount    = cvrp.numberOfTrucks;

    vector< vector<double> > dist(nodeCount + 1, vector<double>(nodeCount + 1, 0.0));
    for (int i = 1; i <= nodeCount; i++) {
        for (int j = 1; j <= nodeCount; j++) {
            dist[i][j] = distanceTo(cvrp.nodes[i], cvrp.nodes[j]);
        }
    }

    vector<Saving> savingList;
    savingList.reserve((nodeCount - 1) * (nodeCount - 1) * 2);

    for (int i = 1; i <= nodeCount; i++) {
        if (i == depot) continue;
        for (int j = i + 1; j <= nodeCount; j++) {
            if (j == depot) continue;
            double savingVal = dist[depot][i] + dist[depot][j] - dist[i][j];

            Saving s1;
            s1.startNode   = i;
            s1.endNode     = j;
            s1.savingCost  = savingVal;

            Saving s2;
            s2.startNode   = j;
            s2.endNode     = i;
            s2.savingCost  = savingVal;

            savingList.push_back(s1);
            savingList.push_back(s2);
        }
    }
    sort(savingList.begin(), savingList.end(), compareSavingCost);

    // 3) Chuẩn bị các biến: allRoutes, belongs, routeDemand
    int numberOfRoutes = nodeCount - 1;
    vector< vector<int> > routeList(nodeCount + 1);
    vector<int> routeOf(nodeCount + 1, -1);
    vector<int> routeDemandUsed(nodeCount + 1, 0);

    // Mỗi node -> 1 route
    int routeIDCounter = 0;
    for (int i = 1; i <= nodeCount; i++) {
        if (i == depot) continue;
        routeIDCounter++;
        routeList[routeIDCounter].push_back(depot);
        routeList[routeIDCounter].push_back(i);
        routeList[routeIDCounter].push_back(depot);

        routeOf[i] = routeIDCounter;
        routeDemandUsed[routeIDCounter] = cvrp.demands[i];
    }

    // 4) Duyệt savingList
    for (int index = 0; index < savingList.size(); index++) {
        if (numberOfRoutes == truckCount) {
            break;
        }

        Saving currentSaving = savingList[index];
        int nodeStart  = currentSaving.startNode;
        int nodeEnd    = currentSaving.endNode;
        double savingC = currentSaving.savingCost;

        int routeStart = routeOf[nodeStart];
        int routeEnd   = routeOf[nodeEnd];

        if (routeStart < 0 || routeEnd < 0) continue;
        if (routeStart == routeEnd) continue;
        if (savingC <= 0) break;

        // Lấy route rStart, rEnd
        vector<int> &routeST = routeList[routeStart];
        vector<int> &routeEN = routeList[routeEnd];

        // Check "nodeStart" ở cuối [.., nodeStart, depot]
        // Check "nodeEnd" ở đầu  [depot, nodeEnd, ..]
        bool stEnd = false;
        bool enStart = false;

        if ((int)routeST.size() >= 2) {
            // routeST[routeST.size()-2] == nodeStart, routeST.back()==depot
            int lastIndexST = (int)routeST.size() - 1;
            if (routeST[lastIndexST] == depot &&
                routeST[lastIndexST - 1] == nodeStart)
            {
                stEnd = true;
            }
        }

        if ((int)routeEN.size() >= 2) {
            // routeEN[0]==depot, routeEN[1]== nodeEnd
            if (routeEN[0] == depot && routeEN[1] == nodeEnd) {
                enStart = true;
            }
        }

        // capacity check
        if (stEnd && enStart) {
            if (routeDemandUsed[routeStart] + routeDemandUsed[routeEnd] <= capacity) {
                // Gộp
                routeDemandUsed[routeStart] += routeDemandUsed[routeEnd];
                routeDemandUsed[routeEnd]   = 0;
                numberOfRoutes--;

                // Pop depot cuối routeST
                routeST.pop_back();
                // Nối [1..end] routeEN
                int routeENsize = (int)routeEN.size();
                for (int k = 1; k < routeENsize; k++) {
                    routeST.push_back(routeEN[k]);
                    if (routeEN[k] != depot) {
                        routeOf[ routeEN[k] ] = routeStart;
                    }
                }
                routeEN.clear();
            }
        }
    }

    double totalCost = 0.0;
    for (int rid = 1; rid <= routeIDCounter; rid++) {
        if (!routeList[rid].empty()) {
            double costVal = calculateRouteCost(routeList[rid], cvrp.nodes);
            totalCost += costVal;
            cvrp.routes.push_back(routeList[rid]);
        }
    }

    cvrp.lengthOfRoutes = totalCost;
    printClarkeWrightResult(cvrp, "No optimization heuristic");
}
