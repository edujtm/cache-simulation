//
// Created by eduardo on 08/05/18.
//

#ifndef CACHESIMULATION_WRITEPOLICY_H
#define CACHESIMULATION_WRITEPOLICY_H

#include<vector>
#include <utility>
#include <cstdio>
#include "../caches/Cache.h"

class CacheLine;

class WritePolicy {
public:
    virtual bool write(std::vector<CacheLine> & set, std::vector<bool> & valid, std::vector<bool> & dirty, size_t offset, uint32_t tag, int8_t data) = 0;
    virtual std::vector<int8_t> recoverFromMemory(size_t blockidx) = 0;
    virtual void writeToMemory(const CacheLine & block, uint32_t blockidx) = 0;
};

#endif //CACHESIMULATION_WRITEPOLICY_H
