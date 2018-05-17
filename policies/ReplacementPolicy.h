//
// Created by eduardo on 08/05/18.
//

#ifndef CACHESIMULATION_REPLACEMENTPOLICY_H
#define CACHESIMULATION_REPLACEMENTPOLICY_H

#include <vector>
#include <cstdint> // uint32_t
#include <cstdlib> // size_t

class ReplacementPolicy {
public:
    virtual size_t getBlockIndex(std::vector<bool> & valid, std::vector<uint32_t > & counters) = 0;
    virtual void update(std::vector<uint32_t> & counters, size_t idx) = 0;
};


#endif //CACHESIMULATION_REPLACEMENTPOLICY_H
