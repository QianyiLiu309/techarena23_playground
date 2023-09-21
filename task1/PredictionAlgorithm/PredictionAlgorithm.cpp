/*
 *
 * 1) bool predictTimeOfDayOnNextPlanet(std::uint64_t nextPlanetID);
 *
 * 2) void observeAndRecordTimeofdayOnNextPlanet(std::uint64_t nextPlanetID, bool timeOfDayOnNextPlanet);
 *
 */

//Sample implementation

#include <unistd.h>
#include "PredictionAlgorithm.hpp"

bool predictTimeOfDayOnNextPlanet(std::uint64_t nextPlanetID, bool spaceshipComputerPrediction) {
  // First option: just follow spaceship computer's suggestions
  return spaceshipComputerPrediction;
  // Another option: predict always as "night" (0)
  // return false;
}

void observeAndRecordTimeofdayOnNextPlanet(std::uint64_t nextPlanetID, bool timeOfDayOnNextPlanet) {
  //default: do nothing
}
