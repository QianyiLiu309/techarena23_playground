// This file contains a template for the implementation of Robo prediction
// algorithm
// Using two ID for searching

#include "PredictionAlgorithm.hpp"
#include <map>
#include <queue>

struct RoboPredictor::RoboMemory {

  std::uint64_t currentPlanetID = 0;
  bool currentPlanetTime = false;
  std::uint64_t lastPlanetID = 0;
  bool lastPlanetTime = false;

  std::map<std::pair<std::uint64_t, std::uint64_t>, std::uint16_t> total_records;
  std::map<std::pair<std::uint64_t, std::uint64_t>, std::uint16_t> daytime_records;
};

bool RoboPredictor::predictTimeOfDayOnNextPlanet(
    std::uint64_t nextPlanetID, bool spaceshipComputerPrediction) {
  // Robo can consult data structures in its memory while predicting.
  // Example: access Robo's memory with roboMemory_ptr-><your RoboMemory
  // content>

  // Robo can perform computations using any data in its memory during
  // prediction. It is important not to exceed the computation cost threshold
  // while making predictions and updating RoboMemory. The computation cost of
  // prediction and updating RoboMemory is calculated by the playground
  // automatically and printed together with accuracy at the end of the
  // evaluation (see main.cpp for more details).

  // Simple prediction policy: follow the spaceship computer's suggestions

  std::map<std::pair<std::uint64_t, std::uint64_t>, std::uint16_t >& total_records = roboMemory_ptr->total_records;
  std::map<std::pair<std::uint64_t, std::uint64_t>, std::uint16_t >& daytime_records = roboMemory_ptr->daytime_records;
  // keep this to calculate the ratio -> hyperparameters
  
  std::uint64_t lastPlanetPattern = (roboMemory_ptr->lastPlanetID << 1 |roboMemory_ptr->lastPlanetTime) << 21 |
                                     roboMemory_ptr->currentPlanetID << 1 | roboMemory_ptr->currentPlanetTime;

  std::pair<std::uint64_t, std::uint64_t> pair = std::make_pair(lastPlanetPattern, nextPlanetID);

  if (total_records.find(pair) == total_records.end()) {
    return spaceshipComputerPrediction;
  }
  else if (float(daytime_records[pair])/float(total_records[pair]) >= 0.5){
    return 1;
  }else{
    return 0;
  }
}

void RoboPredictor::observeAndRecordTimeofdayOnNextPlanet(
    std::uint64_t nextPlanetID, bool timeOfDayOnNextPlanet) {
  // Robo can consult/update data structures in its memory
  // Example: access Robo's memory with roboMemory_ptr-><your RoboMemory
  // content>

  // It is important not to exceed the computation cost threshold while making
  // predictions and updating RoboMemory. The computation cost of prediction and
  // updating RoboMemory is calculated by the playground automatically and
  // printed together with accuracy at the end of the evaluation (see main.cpp
  // for more details).


  std::map<std::pair<std::uint64_t, std::uint64_t>, std::uint16_t>& total_records = roboMemory_ptr->total_records;
  std::map<std::pair<std::uint64_t, std::uint64_t>, std::uint16_t>& daytime_records = roboMemory_ptr->daytime_records;

  std::uint64_t lastPlanetPattern = (roboMemory_ptr->lastPlanetID << 1 |roboMemory_ptr->lastPlanetTime) << 21 |
                                     roboMemory_ptr->currentPlanetID << 1 | roboMemory_ptr->currentPlanetTime;

  std::pair<std::uint64_t, std::uint64_t> pair = std::make_pair(lastPlanetPattern, nextPlanetID);
  
  roboMemory_ptr->lastPlanetID = roboMemory_ptr->currentPlanetID;
  roboMemory_ptr->lastPlanetTime = roboMemory_ptr->currentPlanetTime;

  roboMemory_ptr->currentPlanetID = nextPlanetID;
  roboMemory_ptr->currentPlanetTime = timeOfDayOnNextPlanet;

  if (total_records.find(pair) == total_records.end()) {
    total_records[pair] = 0;
    daytime_records[pair] = 0;
  }

  total_records[pair] += 1;

  if(timeOfDayOnNextPlanet == 1){
    daytime_records[pair] += 1;
  }
  
}


//------------------------------------------------------------------------------
// Please don't modify this file below
//
// Check if RoboMemory does not exceed 64KiB
static_assert(
    sizeof(RoboPredictor::RoboMemory) <= 65536,
    "Robo's memory exceeds 65536 bytes (64KiB) in your implementation. "
    "Prediction algorithms using so much "
    "memory are ineligible. Please reduce the size of your RoboMemory struct.");

// Declare constructor/destructor for RoboPredictor
RoboPredictor::RoboPredictor() {
  roboMemory_ptr = new RoboMemory;
}
RoboPredictor::~RoboPredictor() {
  delete roboMemory_ptr;
}