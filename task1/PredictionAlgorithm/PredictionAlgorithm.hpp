/*
 * This file must declare the following structures and methods.
 * Please don't modify this file.
 */

#include <cstdint>

struct RoboPredictor {
  struct RoboMemory;
  RoboMemory* roboMemory_ptr;

  bool predictTimeOfDayOnNextPlanet(std::uint64_t nextPlanetID,
                                    bool spaceshipComuterPrediction);

  void observeAndRecordTimeofdayOnNextPlanet(std::uint64_t nextPlanetID,
                                             bool timeOfDayOnNextPlanet);
};
