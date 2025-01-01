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
    double oldCost =
          distMatrix[route[j]][route[j+1]]
        + distMatrix[route[k]][route[k+1]]
        + distMatrix[route[l]][route[l+1]];

    double newCost = 0.0;

    // Rút gọn ký hiệu
    int nj = route[j], nj1 = route[j+1];
    int nk = route[k], nk1 = route[k+1];
    int nl = route[l], nl1 = route[l+1];

    switch(typeCase) {
    case 1:
        // "Đảo j+1..k"
        // => newEdges: dis[nj][nk] + dis[nj1][nk1] + dis[nl][nl1]
        newCost = distMatrix[nj][nk]
                + distMatrix[nj1][nk1]
                + distMatrix[nl][nl1];
        break;
    case 2:
        // "Đảo k+1..l"
        // => newEdges: dis[nk][nl] + dis[nk1][nl1] + dis[nj][nj1]
        newCost = distMatrix[nk][nl]
                + distMatrix[nk1][nl1]
                + distMatrix[nj][nj1];
        break;
    case 3:
        // "Đảo (j+1..k) và (k+1..l)"
        newCost = distMatrix[nj][nk]
                + distMatrix[nj1][nl]
                + distMatrix[nk1][nl1];
        break;
    case 4:
        newCost = distMatrix[nj][nk1]
                + distMatrix[nl][nj1]
                + distMatrix[nk][nl1];
        break;
    case 5:
        newCost = distMatrix[nj][nl]
                + distMatrix[nk1][nj1]
                + distMatrix[nk][nl1];
        break;
    case 6:
        newCost = distMatrix[nj][nk1]
                + distMatrix[nl][nk]
                + distMatrix[nj1][nl1];
        break;
    case 7:
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
    // j+1..k, k+1..l => tùy logic
    if(typeCase == 1) {
        // Đảo (j+1..k)
        reverse(route.begin()+j+1, route.begin()+k+1);
    }
    else if(typeCase == 2) {
        // Đảo (k+1..l)
        reverse(route.begin()+k+1, route.begin()+l+1);
    }
    else if(typeCase == 3) {
        // Đảo (j+1..k), rồi (k+1..l)
        reverse(route.begin()+j+1, route.begin()+k+1);
        reverse(route.begin()+k+1, route.begin()+l+1);
    }
    else if(typeCase == 4) {
        // Đảo (j+1..l), rồi chia nhỏ
        reverse(route.begin()+j+1, route.begin()+l+1);
        reverse(route.begin()+j+1, route.begin()+j+1+(l-k));
        reverse(route.begin()+j+1+(l-k), route.begin()+l+1);
    }
    else if(typeCase == 5) {
        reverse(route.begin()+j+1, route.begin()+l+1);
        reverse(route.begin()+j+1 + (l-k), route.begin()+l+1);
    }
    else if(typeCase == 6) {
        reverse(route.begin()+j+1, route.begin()+l+1);
        reverse(route.begin()+j+1, route.begin()+j+1 + (l-k));
    }
    else if(typeCase == 7) {
        reverse(route.begin()+j+1, route.begin()+l+1);
    }
}

void optimizeThreeOptRoute(vector<int> &route, vector<Coord> &nodes)
{
    if(route.size() < 5) {
        return;
    }

    int nN = nodes.size();
    vector<vector<double>> distMatrix(nN, vector<double>(nN, 0.0));
    for(int i=0; i<nN; i++){
        for(int j=0; j<nN; j++){
            distMatrix[i][j] = distanceTo(nodes[i], nodes[j]);
        }
    }

    bool improved;
    do {
        improved = false;
        double bestImprovement = 0.0;
        tuple<int,int,int,int> bestChoice(-1,-1,-1,-1);
        int rSize = route.size();
        for(int i = 0; i < rSize - 3; i++){
            for(int j = i + 1; j < rSize-2; j++){
                for(int k = j + 1; k < rSize-1; k++){
                    for(int tCase=1; tCase<=7; tCase++){
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

            // Log nếu cần
            std::cout << "[3-Opt] Improve=" << bestImprovement
                      << " (i = " << bi << ", j =" << bj
                      << ", k =" << bk << ",type=" << btype << ")\n";
        }

    } while(improved);
}

void runThreeOptFromTwoOpt(CVRP &cvrp)
{
    double originalCost = 0;
    double newCost = 0;
    for(auto &r: cvrp.routes){
        double costB = calculateRouteCost(r, cvrp.nodes);
        originalCost += costB;
        optimizeThreeOptRoute(r, cvrp.nodes);
        double costA = calculateRouteCost(r, cvrp.nodes);
        newCost += costA;
    }

    cvrp.lengthOfRoutes = newCost;
    printClarkeWrightResult(cvrp, "ThreeOpt");
}
