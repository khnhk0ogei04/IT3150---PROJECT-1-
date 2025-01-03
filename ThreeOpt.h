#ifndef THREEOPT_H_INCLUDED
#define THREEOPT_H_INCLUDED
#include "CVRP.h"
#include<iostream>
#include<vector>

using namespace std;

void optimizeThreeOptRoute(vector<int> &route, vector<Node> &nodes);
void runThreeOptFromTwoOpt(CVRP &cvrp);

#endif // THREEOPT_H_INCLUDED
