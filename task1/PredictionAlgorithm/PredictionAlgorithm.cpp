// This file contains a template for the implementation of Robo prediction
// algorithm

#include "PredictionAlgorithm.hpp"
#include <map>
#include <bitset>

#define HISTORY_LENGTH 4
#define COUNTER_LENGTH 2

typedef std::pair<std::uint64_t, std::uint64_t> Branch;
// typedef std::pair<Branch, std::bitset<HISTORY_LENGTH>> KeyToPHT;

struct HistoryPatternComparator {
    bool operator() (
        const std::pair<Branch, std::bitset<HISTORY_LENGTH>> &b1, 
        const std::pair<Branch, std::bitset<HISTORY_LENGTH>> &b2
    ) const {
        if (b1.first == b2.first){
            return b1.second.to_ulong() < b2.second.to_ulong();
        }
        return b1.first < b2.first;
    }
};

// void incrementCounter(std::bitset<COUNTER_LENGTH> &counter){
//     bool carry = true; // add 1
//     for (int i = 0; i < counter.size(); i ++){
//         bool currentBit = counter[i];
//         counter[i] = currentBit ^ carry;
//         carry = carry && currentBit;
//     }
// }

// void decrementCounter(std::bitset<COUNTER_LENGTH> &counter){
//     bool borrow = true;
//     for (int i = 0; i < counter.size(); i ++){
//         bool currentBit = counter[i];
//         counter[i] = currentBit ^ borrow;
//         borrow = borrow && !currentBit;
//     }
// }

typedef std::map<Branch, std::bitset<HISTORY_LENGTH>> BranchHistoryTable;
typedef std::map<std::pair<Branch, std::bitset<HISTORY_LENGTH>>, std::bitset<COUNTER_LENGTH>, HistoryPatternComparator> PatternHistoryTable;

struct RoboPredictor::RoboMemory {
    // Place your RoboMemory content here
    // Note that the size of this data structure can't exceed 64KiB!

    // store 4 bits of address 
    BranchHistoryTable branchHistoryTable; 
    // std::map<Branch, std::uint64_t> branchOccurrenceCounter;

    // use a 2-bit saturating counter 
    PatternHistoryTable patternHistoryTable;
    std::uint64_t previousPlanetId = 0; 
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
    Branch branch = std::make_pair(roboMemory_ptr->previousPlanetId, nextPlanetID); 
    
    BranchHistoryTable &branchHistoryTable = roboMemory_ptr->branchHistoryTable;
    PatternHistoryTable &patternHistoryTable = roboMemory_ptr->patternHistoryTable;

    if (branchHistoryTable.find(branch) == branchHistoryTable.end() || patternHistoryTable.find(std::make_pair(branch, branchHistoryTable[branch])) == patternHistoryTable.end()) {
        // if branch history entry or pattern history entry not found, just follow spaceship computer's prediction
        // this can happen when it occurs less than HISTORY_LENGTH times 
        return spaceshipComputerPrediction; 
    } 
    else{ 
        std::pair<Branch, std::bitset<HISTORY_LENGTH>> keyToPHT = std::make_pair(branch, branchHistoryTable[branch]); 
        
        // use MSB of counter as prediction
        std::bitset<COUNTER_LENGTH> &counter = patternHistoryTable[keyToPHT]; 
        std::bitset<COUNTER_LENGTH> counterMSBMask;
        counterMSBMask.set(counter.size() - 1);
        return (counter & counterMSBMask).to_ulong() > 0; 
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
    Branch branch = std::make_pair(roboMemory_ptr->previousPlanetId, nextPlanetID); 
    
    BranchHistoryTable &branchHistoryTable = roboMemory_ptr->branchHistoryTable;
    PatternHistoryTable &patternHistoryTable = roboMemory_ptr->patternHistoryTable;
    // std::map<Branch, std::uint64_t> &branchOccurrenceCounter = roboMemory_ptr->branchOccurrenceCounter; 
    
    if (branchHistoryTable.find(branch) == branchHistoryTable.end()) { 
        // just create the branchHistory entry, don't create counter until branchHistory is full
        branchHistoryTable[branch] = std::bitset<HISTORY_LENGTH>(); 
        // roboMemory_ptr->branchOccurrenceCounter.insert(std::make_pair(branch, 0));

        std::pair<Branch, std::bitset<HISTORY_LENGTH>> keyToPHT = std::make_pair(branch, branchHistoryTable[branch]);
        patternHistoryTable[keyToPHT] = std::bitset<COUNTER_LENGTH>();
    } 
    // else if (branchOccurrenceCounter[branch] >= HISTORY_LENGTH){
    else{
        // only update branch pattern history when enough history is collected for the branch history entry
        std::pair<Branch, std::bitset<HISTORY_LENGTH>> keyToPHT = std::make_pair(branch, branchHistoryTable[branch]);
        std::bitset<COUNTER_LENGTH> &counter = patternHistoryTable[keyToPHT]; 
        if (counter.any() && !timeOfDayOnNextPlanet){ 
            // if counter > 0 and branch not taken 
            counter = counter.to_ulong() - 1; 
            // decrementCounter(counter);
        } 
        else if (!counter.all() && timeOfDayOnNextPlanet){ 
            // if counter < max and branch taken 
            counter = counter.to_ulong() + 1; 
            // incrementCounter(counter);
        }
    }

    std::bitset<HISTORY_LENGTH> newHistoryPattern = branchHistoryTable[branch] << 1;
    if (timeOfDayOnNextPlanet){
        newHistoryPattern.set(0);
    } 
    branchHistoryTable[branch] = newHistoryPattern;
    // roboMemory_ptr->branchOccurrenceCounter[branch] += 1;

    // if (roboMemory_ptr->branchOccurrenceCounter[branch] == HISTORY_LENGTH){
    //     std::pair<Branch, std::bitset<HISTORY_LENGTH>> keyToPHT = std::make_pair(branch, branchHistoryTable[branch]);
    //     patternHistoryTable[keyToPHT] = std::bitset<COUNTER_LENGTH>();
    // }

    roboMemory_ptr->previousPlanetId = nextPlanetID; 
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
