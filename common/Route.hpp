/*
 *   BSD LICENSE
 *
 *   (C) Copyright 2023 Huawei Technologies Research & Development (UK) Ltd
 *   All rights reserved.
 *   Prepared by Artemiy Margaritov <artemiy.margaritov@huawei.com>
 */

#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60
void printProgress(double percentage) {
  int val = (int)(percentage * 100);
  int lpad = (int)(percentage * PBWIDTH);
  int rpad = PBWIDTH - lpad;
  printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
  fflush(stdout);
}

struct PlanetInfo {
  std::uint64_t planetID;
  bool timeOfDay;
};

bool convertTimeOfDayToBool(std::string timeOfDay) {
  if (timeOfDay == "DAY") return true;
  if (timeOfDay == "NIGHT") return false;
  return false;
}

struct Route {
  std::string route_filename;
  std::ifstream route_file;
  int totalNumberOfPlanets;
  int numberOfVisitedPlanets;
  int numberOfCorrectPredictions;

  Route(std::string& filename) {
    numberOfVisitedPlanets = 0;
    numberOfCorrectPredictions = 0;

    // Count number of lines in the input
    std::string curLine;
    int lineCount = 0;
    std::ifstream file(filename);
    while (getline(file, curLine)) lineCount++;
    totalNumberOfPlanets = lineCount - 1;  // ignore the header line

    route_filename = filename;
    route_file.open(filename, std::ifstream::in);
    // Ignore the header line
    std::getline(route_file, curLine);
  }

  uint64_t getTotalNumberOfPlanets() { return totalNumberOfPlanets; }

  bool readLineFromFile(PlanetInfo& planet) {
    if (!route_file.is_open()) {
      std::cerr << "Error: Could not open route file " << route_filename
                << std::endl << std::endl;
      return false;
    }

    if (!route_file.good()) {
      return false;
    }
    std::string line;
    if (std::getline(route_file, line)) {
      // Find the position of the tab character
      size_t tabPos = line.find('\t');

      if (tabPos != std::string::npos) {
        // Extract the planet ID from the beginning of the line
        planet.planetID = std::stoul(line.substr(0, tabPos));

        // Extract the string value after the tab character
        std::string str = line.substr(tabPos + 1);

        if ((str != "DAY") && (str != "NIGHT")) {
          std::cerr << "Error: Could not parse time-of-day for planet "
                    << planet.planetID << ". Received time-of-day " << str
                    << std::endl;
          return false;  // incorrect format
        }

        // Extract time-of-day
        planet.timeOfDay = convertTimeOfDayToBool(str);
        numberOfVisitedPlanets++;

        return true;
      }
    }
    return false;  // Line not found or incorrect format
  }

  void displayProgressBar() {
    if ((numberOfVisitedPlanets & 1023) == 0) {
      printProgress(float(numberOfVisitedPlanets) / totalNumberOfPlanets);
    }
  }

  void updatePredictionAccuracyStatistics(bool prediction,
                                          bool correctOutcome) {
    numberOfCorrectPredictions += (prediction == correctOutcome);
  }

  void printFinalPredictionAccuracy() {
    std::cout << std::endl;
    std::cout << "Total number of planets visited " << numberOfVisitedPlanets
              << std::endl;
    std::cout << "Prediction accuracy " << std::setprecision(4)
              << (float)numberOfCorrectPredictions / totalNumberOfPlanets * 100
              << "%" << std::endl;
  }
};
