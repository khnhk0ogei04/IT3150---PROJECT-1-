#ifndef _CLARKEWRIGHT_H
#define _CLARKEWRIGHT_H

#include "CVRP.h"
#include <iostream>

using namespace std;

struct Saving {
    int startNode;
    int endNode;
    double savingCost;
};

void runClarkeWright(CVRP& cvrp);

#endif
