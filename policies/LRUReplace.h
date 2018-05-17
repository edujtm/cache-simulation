//
// Created by eduardo on 08/05/18.
//

#ifndef CACHESIMULATION_LRUREPLACE_H
#define CACHESIMULATION_LRUREPLACE_H


#include "ReplacementPolicy.h"
#include <vector>

class LRUReplace : public ReplacementPolicy {

    // This value may overflow, but it'll not for the purposes of the simulation
    uint32_t now;
public:
    LRUReplace();
    size_t getBlockIndex(std::vector<bool> & valid, std::vector<uint32_t> & counters) override;
    void update(std::vector<uint32_t> & counters, size_t idx) override;
};


#endif //CACHESIMULATION_LRUREPLACE_H
