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

    std::vector<std::string> vrpFiles;
    for (const auto &entry : fs::directory_iterator(inputDirectory)) {
        if (entry.path().extension() == ".vrp") {
            vrpFiles.push_back(entry.path().string());
        }
    }

    for (const std::string &filePath : vrpFiles) {
        std::cout << "Processing file: " << filePath << std::endl;

        CVRP cvrp = readFile(filePath);

        runClarkeWright(cvrp);
        writeResultToFile(cvrp, outputDirectory + "clarkewright//clarkewright_" + cvrp.name + ".txt");

        runTwoOptFromClarkeWright(cvrp);
        writeResultToFile(cvrp, outputDirectory + "twoopt//twoopt_" + cvrp.name + ".txt");

        runThreeOptFromTwoOpt(cvrp);
        writeResultToFile(cvrp, outputDirectory + "threeopt//threeopt_" + cvrp.name + ".txt");

        std::cout << "Finished processing file: " << filePath << std::endl;
    }

    return 0;
}
