#include "ClarkeWright.h"
#include "CVRP.h"
#include "TwoOpt.h"
#include "ThreeOpt.h"
#include <iostream>

int main() {
    std::string filePath = "C://Users//Lenovo//Downloads//Vrp-Set-A//A//A-n32-k5.vrp";
    std::string outputDirectory = "C://Users//Lenovo//Downloads//Vrp-Set-A//";

    CVRP cvrp = readFile(filePath);

    runClarkeWright(cvrp);
    writeResultToFile(cvrp, outputDirectory + "clarkewright//clarkewright_" + cvrp.name + ".txt");

    runTwoOptFromClarkeWright(cvrp);
    writeResultToFile(cvrp, outputDirectory + "twoopt//twoopt_" + cvrp.name + ".txt");

    runThreeOptFromTwoOpt(cvrp);
    writeResultToFile(cvrp, outputDirectory + "threeopt//threeopt_" + cvrp.name + ".txt");

    return 0;
}
