// This file contains a template for the implementation of Robo prediction
// algorithm
// Using two ID for searching

#include "PredictionAlgorithm.hpp"
#include <map>
#include <bitset>
#include <queue>

struct RoboPredictor::RoboMemory {
  uint64_t LastPlanet5 = 0;
  uint64_t LastPlanet2 = 0;
  uint32_t LastPlanet1 = 0;
  uint32_t LastPlanet3 = 0;
  uint32_t LastPlanet4 = 0;
  uint32_t CurrentPlanet = 0;
  std::map<std::pair<std::pair<std::uint64_t,std::uint32_t>, std::uint32_t>, std::int16_t> total_records;
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

  std::map<std::pair<std::pair<std::uint64_t,std::uint32_t>, std::uint32_t>, std::int16_t>& total_records = roboMemory_ptr->total_records;

  std::pair<std::uint64_t,std::uint64_t> lastPlanetPattern = std::make_pair((roboMemory_ptr->LastPlanet5<< 42) | (roboMemory_ptr->LastPlanet4 << 21) |
                                    (roboMemory_ptr->LastPlanet3), (roboMemory_ptr->LastPlanet2<< 42) | (roboMemory_ptr->LastPlanet1 << 21) |
                                    (roboMemory_ptr->CurrentPlanet));

  std::pair<std::pair<std::uint64_t,std::uint64_t>, std::uint32_t> pair = std::make_pair(lastPlanetPattern, nextPlanetID);

  if (total_records.find(pair) == total_records.end()) {
    return spaceshipComputerPrediction;
  }
  else if (total_records[pair] > 0){
    return 1;
  }
  else if (total_records[pair] <= 0) {
    return 0;
  }
  else{
    return spaceshipComputerPrediction;
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


  std::map<std::pair<std::pair<std::uint64_t,std::uint32_t>, std::uint32_t>, std::int16_t>& total_records = roboMemory_ptr->total_records;

  std::pair<std::uint64_t,std::uint64_t> lastPlanetPattern = std::make_pair((roboMemory_ptr->LastPlanet5<< 42) | (roboMemory_ptr->LastPlanet4 << 21) |
                                    (roboMemory_ptr->LastPlanet3), (roboMemory_ptr->LastPlanet2<< 42) | (roboMemory_ptr->LastPlanet1 << 21) |
                                    (roboMemory_ptr->CurrentPlanet));

  std::pair<std::pair<std::uint64_t,std::uint64_t>, std::uint32_t> pair = std::make_pair(lastPlanetPattern, nextPlanetID);
  
  roboMemory_ptr->LastPlanet5 = roboMemory_ptr->LastPlanet4;
  roboMemory_ptr->LastPlanet4 = roboMemory_ptr->LastPlanet3;
  roboMemory_ptr->LastPlanet3 = roboMemory_ptr->LastPlanet2;
  roboMemory_ptr->LastPlanet2 = roboMemory_ptr->LastPlanet1;
  roboMemory_ptr->LastPlanet1 = roboMemory_ptr->CurrentPlanet;

  roboMemory_ptr->CurrentPlanet = nextPlanetID << 1 | timeOfDayOnNextPlanet;

  if (total_records.find(pair) == total_records.end()) {
    total_records[pair] = 0;
  }

  if(timeOfDayOnNextPlanet == 1){
    total_records[pair] += 1;
  }else{
    total_records[pair] -= 1;
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