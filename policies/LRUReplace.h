//
// Created by eduardo on 08/05/18.
//

#ifndef CACHESIMULATION_LRUREPLACE_H
#define CACHESIMULATION_LRUREPLACE_H


#include "ReplacementPolicy.h"
#include <vector>

class LRUReplace : public ReplacementPolicy {

    // This value may overflow, but it'll not for the purposes of the simulation
    int now;
public:
    LRUReplace(): now{0} {}
    int getBlockIndex(std::vector<bool> & valid, std::vector<int> & counters) override;
    void update(std::vector<int> & counters, int idx) override;
};


#endif //CACHESIMULATION_LRUREPLACE_H
