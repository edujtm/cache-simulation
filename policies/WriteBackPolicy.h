//
// Created by eduardo on 08/05/18.
//

#ifndef CACHESIMULATION_WRITEBACKPOLICY_H
#define CACHESIMULATION_WRITEBACKPOLICY_H

#include <vector>
#include "WritePolicy.h"
#include "../caches/Cache.h"

class WriteBackPolicy : public WritePolicy {

    std::vector<std::vector<int8_t > > & memory;
public:
    explicit WriteBackPolicy(std::vector<std::vector<int8_t >> & mem);
    bool write(std::vector<CacheLine> & set, std::vector<bool> & valid, std::vector<bool> & dirty, size_t offset, uint32_t tag, int8_t value ) override;
    std::vector<int8_t> recoverFromMemory(size_t blockidx) override;
    void writeToMemory(const CacheLine & block, uint32_t blockidx) override;
};


#endif //CACHESIMULATION_WRITEBACKPOLICY_H
