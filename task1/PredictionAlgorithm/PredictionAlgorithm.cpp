// This file contains a template for the implementation of Robo prediction
// algorithm

#include "PredictionAlgorithm.hpp"
#include <map>
#include <set>
#include <queue>

struct RoboPredictor::RoboMemory {
  // Place your RoboMemory content here
  // Note that the size of this data structure can't exceed 64KiB!

  // bool lastPlannetIsDayTime = true;
  // std::map<std::uint64_t, bool> planetTimeOfDay;

  std::uint64_t currentPlanetID = 0;
  bool currentPlanetTime = false;
  std::uint64_t lastPlanetID = 0;
  bool lastPlanetTime = false;

  // std::map<std::uint64_t, std::set<std::uint64_t>> correlatedPlanets;

  std::map<std::pair<std::uint64_t, std::uint64_t>, std::deque<bool>> records;
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

  std::map<std::pair<std::uint64_t, std::uint64_t>, std::deque<bool>>& records = roboMemory_ptr->records;

  std::pair<std::uint64_t, std::uint64_t> pair = std::make_pair(roboMemory_ptr->currentPlanetID, nextPlanetID);

  if (records.find(pair) == records.end() || records[pair].size() != 10) {
    return spaceshipComputerPrediction;
  }
  else {
    std::uint64_t count = 0;
    for (bool b : records[pair]) {
      if (b) {
        count++;
      }
    }
    if (count >= 8){
      return roboMemory_ptr->currentPlanetTime;
    }
    else {
      return spaceshipComputerPrediction;
    } 
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

  // Simple prediction policy: do nothing

  roboMemory_ptr->lastPlanetID = roboMemory_ptr->currentPlanetID;
  roboMemory_ptr->lastPlanetTime = roboMemory_ptr->currentPlanetTime;

  roboMemory_ptr->currentPlanetID = nextPlanetID;
  roboMemory_ptr->currentPlanetTime = timeOfDayOnNextPlanet;

  std::map<std::pair<std::uint64_t, std::uint64_t>, std::deque<bool>>& records = roboMemory_ptr->records;

  std::pair<std::uint64_t, std::uint64_t> pair = std::make_pair(roboMemory_ptr->lastPlanetID, roboMemory_ptr->currentPlanetID);
  
  if (records.find(pair) == records.end()) {
    records[pair] = std::deque<bool>();
  }
  if (records[pair].size() == 10) {
    records[pair].pop_front();
  }
  records[pair].push_back(roboMemory_ptr->currentPlanetTime == roboMemory_ptr->lastPlanetTime);
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
