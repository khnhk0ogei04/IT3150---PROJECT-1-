#include "ClarkeWright.h"
#include "CVRP.h"
#include "TwoOpt.h"
#include "ThreeOpt.h"
#include <iostream>
#include <vector>
#include <filesystem>

using namespace std;
namespace fs = filesystem;

int main() {
    string inputDirectory = "C://Users//Lenovo//Downloads//Vrp-Set-A//A//";
    string outputDirectory = "C://Users//Lenovo//Downloads//Vrp-Set-A//";
    vector<string> vrpFiles;
    for (const auto &entry : fs::directory_iterator(inputDirectory)) {
        if (entry.path().extension() == ".vrp") {
            vrpFiles.push_back(entry.path().string());
        }
    }
    for (string filePath : vrpFiles) {
        cout << "Processing file: " << filePath << endl;
        CVRP cvrp = readFile(filePath);

        runClarkeWright(cvrp);
        writeResultToFile(cvrp, outputDirectory + "clarkewright//clarkewright_" + cvrp.name + ".txt");

        runTwoOptFromClarkeWright(cvrp);
        writeResultToFile(cvrp, outputDirectory + "twoopt//twoopt_" + cvrp.name + ".txt");

        runThreeOptFromTwoOpt(cvrp);
        writeResultToFile(cvrp, outputDirectory + "threeopt//threeopt_" + cvrp.name + ".txt");

        cout << "Finished processing file: " << filePath << endl;

        std::cout << "Finished processing file: " << filePath << std::endl;
    }

    return 0;
}
