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

<<<<<<< HEAD
    vector<string> vrpFiles;
=======
    std::vector<std::string> vrpFiles;
>>>>>>> c675e3706d3d3c8961a74ee599949fb8f08d60f4
    for (const auto &entry : fs::directory_iterator(inputDirectory)) {
        if (entry.path().extension() == ".vrp") {
            vrpFiles.push_back(entry.path().string());
        }
    }

<<<<<<< HEAD
    for (string filePath : vrpFiles) {
        cout << "Processing file: " << filePath << endl;
=======
    for (const std::string &filePath : vrpFiles) {
        std::cout << "Processing file: " << filePath << std::endl;
>>>>>>> c675e3706d3d3c8961a74ee599949fb8f08d60f4

        CVRP cvrp = readFile(filePath);

        runClarkeWright(cvrp);
        writeResultToFile(cvrp, outputDirectory + "clarkewright//clarkewright_" + cvrp.name + ".txt");

        runTwoOptFromClarkeWright(cvrp);
        writeResultToFile(cvrp, outputDirectory + "twoopt//twoopt_" + cvrp.name + ".txt");

        runThreeOptFromTwoOpt(cvrp);
        writeResultToFile(cvrp, outputDirectory + "threeopt//threeopt_" + cvrp.name + ".txt");

<<<<<<< HEAD
        cout << "Finished processing file: " << filePath << endl;
=======
        std::cout << "Finished processing file: " << filePath << std::endl;
>>>>>>> c675e3706d3d3c8961a74ee599949fb8f08d60f4
    }

    return 0;
}
