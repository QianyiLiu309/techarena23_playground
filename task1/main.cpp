/*
 *   BSD LICENSE
 *
 *   (C) Copyright 2023 Huawei Technologies Research & Development (UK) Ltd
 *   All rights reserved.
 *   Prepared by Artemiy Margaritov <artemiy.margaritov@huawei.com>
 */

#include "CmdlineArgumentParser.hpp"
#include "DynamicInstructionCounting/DynamicInstructionCounting_API.hpp"
#include "Route.hpp"
#include "SpaceshipComputer/SpaceshipComputer.hpp"
#include "PredictionAlgorithm/PredictionAlgorithm.hpp"

int main(int argc, char **argv) {
  // Parse comand-line options
  CmdlineOptions cmdline_opts;
  if (!parseComdlineOptions(argc, argv, cmdline_opts)) {
    std::cout << "Can't parse command-line arguments" << std::endl;
    return 1;
  }
  // open a file with a route and initialize the route structure
  Route route(cmdline_opts.inFile);
  // create and initialize spaceshipComputer object 
  SpaceshipComputer spaceshipComputer;

  PlanetInfo nextPlanet;
  while (route.readLineFromFile(nextPlanet)) {
    // ask spaceship computer for help
    bool spaceshipComputerPrediction = spaceshipComputer.predict(nextPlanet.planetID);  

    // dynamic instruction counting is required to check if compute resource
    // limit was not violated while making prediction and recording the actual
    // outcome
    enableDynamicInstructionCounting();
    // ask spaceship computer for help
    bool spaceshipComputerPrediction = spaceshipComputer.predict(nextPlanet.planetID);  


    //---------The functions called below are to be implemented by contestants-----
    // make a prediction of time-of-day on the next planet
    bool prediction = predictTimeOfDayOnNextPlanet(nextPlanet.planetID, spaceshipComputerPrediction);

    // arrive on the planet and learn the actual time-of-day there.
    // record the patterns in the internal memory
    observeAndRecordTimeofdayOnNextPlanet(nextPlanet.planetID,
                                          nextPlanet.timeOfDay);
    //---------End of the section with the functions that are to be implemented by contestants
    
    
    // dynamic instruction counting is no longer required
    disableDynamicInstructionCounting();
    // notify spaceship computer about the actual time-of-day outcome, so it can update it's internal memory
    spaceshipComputer.update(nextPlanet.planetID, 
                             nextPlanet.timeOfDay);
    
    // update accuracy statistics
    route.updatePredictionAccuracyStatistics(prediction, nextPlanet.timeOfDay);
    // verbose output
    if (cmdline_opts.isVerboseOutputEnabled) {
        std::cout << "Visited planet with ID " << nextPlanet.planetID 
                  << " Predicted time-of-day " << prediction
                  << " Actual observed time-of-day " << nextPlanet.timeOfDay << std::endl;
    } else {
        // not verbose output
        route.displayProgressBar();
    }
  }
  // print total prediction accuracy
  route.printFinalPredictionAccuracy();
  printInstructionCountingStatistics(route.getTotalNumberOfPlanets());
}
