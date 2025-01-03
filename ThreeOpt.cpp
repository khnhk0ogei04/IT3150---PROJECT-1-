#include "ThreeOpt.h"
#include <algorithm>
#include <iostream>
#include <cmath>
#include <tuple>
#include <vector>
#include <map>
#include <string>
#include <limits>

using namespace std;
const double EPSILON = 1e-6;

double computeThreeOptGain(
    const vector<int> &route,
    const vector<vector<double>> &distMatrix,
    int j, int k, int l,
    int typeCase)
{
    double oldCost = distMatrix[route[j]][route[j+1]] + distMatrix[route[k]][route[k+1]] + distMatrix[route[l]][route[l+1]];
    double newCost = 0.0;
    int nj = route[j], nj1 = route[j+1];
    int nk = route[k], nk1 = route[k+1];
    int nl = route[l], nl1 = route[l+1];
    switch(typeCase) {
    case 1:
        // đảo j+1..k
        // newEdges: dis[nj][nk] + dis[nj1][nk1] + dis[nl][nl1]
        newCost = distMatrix[nj][nk]
                + distMatrix[nj1][nk1]
                + distMatrix[nl][nl1];
        break;
    case 2:
        // đảo k+1..l
        // newEdges: dis[nk][nl] + dis[nk1][nl1] + dis[nj][nj1]
        newCost = distMatrix[nj][nj1]
                + distMatrix[nk][nl]
                + distMatrix[nk1][nl1];
        break;
    case 3:
        // đảo (j+1..k) và (k+1..l)
        newCost = distMatrix[nj][nk]
                + distMatrix[nj1][nl]
                + distMatrix[nk1][nl1];
        break;
    case 4:
        // đảo đoạn (j + 1.. l), cho khúc (k + 1, l) lên trên (j + 1, k)
        // Vd: 1 2 3 4 5 6 7 ->  1 5 6 7 2 3 4 8
        newCost = distMatrix[nj][nk1]
                + distMatrix[nl][nj1]
                + distMatrix[nk][nl1];
        break;
    case 5:
        // đảo đoạn (j + 1, l), cho khúc (k + 1, l) lên trên (j + 1, k)
        // sau đó đảo (k + 1, l)
        newCost = distMatrix[nj][nl]
                + distMatrix[nk1][nj1]
                + distMatrix[nk][nl1];
        break;
    case 6:
        // đảo đoạn (j + 1, l), cho khúc (k + 1, l) lên trên (j + 1, k)
        // sau đó đảo (j + 1, k)
        newCost = distMatrix[nj][nk1]
                + distMatrix[nl][nk]
                + distMatrix[nj1][nl1];
        break;
    case 7:
        // đảo cả cục từ (j + 1, l)
        newCost = distMatrix[nj][nl]
                + distMatrix[nk1][nk]
                + distMatrix[nj1][nl1];
        break;
    default:
        return 0.0;
    }
    return (oldCost - newCost);
}
void applyThreeOptCase(vector<int> &route, int j, int k, int l, int typeCase)
{
    vector<int> newRoute;
    if(typeCase == 1) {
        // dảo (j + 1, k)
        for (int i = 0; i <= j; i++) newRoute.push_back(route[i]);
        for (int i = k; i >= j + 1; i--) newRoute.push_back(route[i]);
        for (int i = k + 1; i < route.size(); i++) newRoute.push_back(route[i]);
    }
    else if(typeCase == 2) {
        // đảo (k+1..l)
        for (int i = 0; i <= k; i++) newRoute.push_back(route[i]);
        for (int i = l; i >= k + 1; i--) newRoute.push_back(route[i]);
        for (int i = l + 1; i < route.size(); i++) newRoute.push_back(route[i]);
    }
    else if(typeCase == 3) {
        // đảo (j+1..k) và (k+1..l)
        for (int i = 0; i <= j; i++) newRoute.push_back(route[i]);
        for (int i = k; i >= j + 1; i--) newRoute.push_back(route[i]);
        for (int i = l; i >= k + 1; i--) newRoute.push_back(route[i]);
        for (int i = l + 1; i < route.size(); i++) newRoute.push_back(route[i]);
    }
    else if(typeCase == 4) {
        reverse(route.begin()+j+1, route.begin()+l+1);
        reverse(route.begin()+j+1, route.begin()+j+1+(l-k));
        reverse(route.begin()+j+1+(l-k), route.begin()+l+1);
    }
    else if(typeCase == 5) {
        reverse(route.begin() + j + 1, route.begin() + l + 1);
        reverse(route.begin() + j + 1 + (l - k), route.begin() + l + 1);
    }
    else if(typeCase == 6) {
        reverse(route.begin() + j + 1, route.begin() + l + 1);
        reverse(route.begin() + j + 1, route.begin() + j + 1 + (l - k));
    }
    else if(typeCase == 7) {
        reverse(route.begin() + j + 1, route.begin() + l + 1);
    }
}

void optimizeThreeOptRoute(vector<int> &route, vector<Node> &nodes)
{
    if(route.size() < 5) {
        return;
    }

    int numberOfNodes = nodes.size();
    vector<vector<double>> distMatrix(numberOfNodes, vector<double>(numberOfNodes, 0.0));
    for(int i=0; i< numberOfNodes; i++){
        for(int j=0; j< numberOfNodes; j++){
            distMatrix[i][j] = distanceTo(nodes[i], nodes[j]);
        }
    }

    bool improved;
    do {
        improved = false;
        double bestImprovement = 0.0;
        tuple<int,int,int,int> bestChoice(-1,-1,-1,-1);
        int routeSize = route.size();
        for(int i = 0; i < routeSize - 3; i++){
            for(int j = i + 1; j < routeSize - 2; j++){
                for(int k = j + 1; k < routeSize - 1; k++){
                    for(int tCase = 1; tCase <= 7; tCase++){
                        double gain = computeThreeOptGain(route, distMatrix, i, j, k, tCase);
                        if(gain > bestImprovement + EPSILON){
                            bestImprovement = gain;
                            bestChoice = {i, j, k, tCase};
                        }
                    }
                }
            }
        }

        if(bestImprovement > EPSILON){
            auto [bi, bj, bk, btype] = bestChoice;
            applyThreeOptCase(route, bi, bj, bk, btype);
            improved = true;
        }

    } while(improved);
}

void runThreeOptFromTwoOpt(CVRP &cvrp)
{
    double originalCost = 0;
    double newCost = 0;
    for(vector<int> &r: cvrp.routes){
        double costB = calculateRouteCost(r, cvrp.nodes);
        originalCost += costB;
        optimizeThreeOptRoute(r, cvrp.nodes);
        double costA = calculateRouteCost(r, cvrp.nodes);
        newCost += costA;
    }

    cvrp.lengthOfRoutes = newCost;
    printClarkeWrightResult(cvrp, "ThreeOpt");
}
