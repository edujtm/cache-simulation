//
// Created by eduardo on 08/05/18.
//

#ifndef CACHESIMULATION_REPLACEMENTPOLICY_H
#define CACHESIMULATION_REPLACEMENTPOLICY_H

#include <vector>

class ReplacementPolicy {
public:
    virtual int getBlockIndex(std::vector<bool> & valid, std::vector<int> & counters) = 0;
    virtual void update(std::vector<int> & counters, int idx) = 0;
};


#endif //CACHESIMULATION_REPLACEMENTPOLICY_H
