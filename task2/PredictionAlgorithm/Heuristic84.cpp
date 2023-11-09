// This file contains a template for the implementation of Robo prediction
// algorithm

#include "PredictionAlgorithm.hpp"
#include <map>
#include <set>
#include <queue>
#include <iostream>

struct RoboPredictor::RoboMemory {

  std::uint64_t currentPlanetID = 0;
  std::uint32_t currentGroupID = 0;
  bool currentPlanetTime = false;
  std::uint64_t lastPlanetID = 0;
  std::uint32_t lastGroupID = 0;
  bool lastPlanetTime = false;
  std::uint32_t number = 0;
  std::uint32_t groupacc = 0;
  std::uint32_t planetacc = 0;
  std::map<std::pair<std::uint64_t, std::uint64_t>, std::uint16_t> total_records;
  std::map<std::pair<std::uint64_t, std::uint64_t>, std::uint16_t> same_records;
};

bool RoboPredictor::predictTimeOfDayOnNextPlanet(
    std::uint64_t nextPlanetID, bool spaceshipComputerPrediction, int nextPlanetGroupTag) {
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
  std::map<std::pair<std::uint64_t, std::uint64_t>, std::uint16_t >& same_records = roboMemory_ptr->same_records;


  roboMemory_ptr->lastGroupID = roboMemory_ptr->currentGroupID;
  roboMemory_ptr->currentGroupID = nextPlanetGroupTag;

  std::pair<std::uint64_t, std::uint64_t> planetpair = std::make_pair(roboMemory_ptr->currentPlanetID, nextPlanetID);

  if (total_records.find(planetpair) == total_records.end()) {
    return spaceshipComputerPrediction;
  }
  else {
  
    if (float(same_records[planetpair])/float(total_records[planetpair]) >= 0.5){
      return roboMemory_ptr->currentPlanetTime;
    }
    else if (float(same_records[planetpair])/float(total_records[planetpair]) < 0.5){
      return !(roboMemory_ptr->currentPlanetTime);
    }else{
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

  std::map<std::pair<std::uint64_t, std::uint64_t>, std::uint16_t>& total_records = roboMemory_ptr->total_records;
  std::map<std::pair<std::uint64_t, std::uint64_t>, std::uint16_t>& same_records = roboMemory_ptr->same_records;

  std::pair<std::uint64_t, std::uint64_t> grouppair = std::make_pair(roboMemory_ptr->lastGroupID, roboMemory_ptr->currentGroupID);
  std::pair<std::uint64_t, std::uint64_t> planetpair = std::make_pair(roboMemory_ptr->lastPlanetID, roboMemory_ptr->currentPlanetID);
  if (total_records.find(grouppair) == total_records.end()) {
    total_records[grouppair] = 0;
    same_records[grouppair] = 0;
  }

   if (total_records.find(planetpair) == total_records.end()) {
    total_records[planetpair] = 0;
    same_records[planetpair] = 0;
  }

  total_records[grouppair] += 1;
  total_records[planetpair] += 1;

  if(roboMemory_ptr->lastPlanetTime == roboMemory_ptr->currentPlanetTime){
    if(float(same_records[grouppair])/float(total_records[grouppair]) >= 0.5)
        roboMemory_ptr->groupacc ++;
    same_records[grouppair] += 1;
    if(float(same_records[planetpair])/float(total_records[planetpair]) >= 0.5)
        roboMemory_ptr->planetacc ++;
    same_records[planetpair] += 1;
  }else{
    if(float(same_records[grouppair])/float(total_records[grouppair]) < 0.5)
      roboMemory_ptr->groupacc ++;
    if(float(same_records[planetpair])/float(total_records[planetpair]) < 0.5)
      roboMemory_ptr->planetacc ++;
  }
  
  roboMemory_ptr->number ++;
  
   if(roboMemory_ptr->number % 50000 == 0){
    std::cout << "z " << "Number: " << roboMemory_ptr->number << std::endl;
    std::cout << "GroupAcc: " << float(roboMemory_ptr->groupacc)/float(roboMemory_ptr->number) << std::endl;
    std::cout << "PlanetAcc: " << float(roboMemory_ptr->planetacc)/float(roboMemory_ptr->number) << std::endl;
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