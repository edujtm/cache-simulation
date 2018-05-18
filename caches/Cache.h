//
// Created by eduardo on 08/05/18.
//

#ifndef CACHESIMULATION_DIRECTMAPCACHE_H
#define CACHESIMULATION_DIRECTMAPCACHE_H

#include "../policies/ReplacementPolicy.h"
#include "../helper/HitMissCounter.h"
#include "../policies/WritePolicy.h"
#include "../policies/LRUReplace.h"
#include <cmath>
#include <vector>
#include <utility>

#define BLOCKSIZE 16u

struct CacheLine {
    uint32_t tag;
    std::vector<int8_t> data;

    CacheLine() {
        tag = 0;
        data = std::vector<int8_t>(BLOCKSIZE, 0);
    }
};

class WritePolicy;
class ReplacementPolicy;

class Cache {
    uint32_t nsets;
    uint32_t cachesize;
    uint32_t assoc;
    uint32_t blocksize;
    uint32_t offsetBitCount;
    uint32_t indexBitCount;

    ReplacementPolicy * replace;
    WritePolicy * writePolicy;

    // Vetor de sets de blocos de memoria cache
    std::vector<std::vector<CacheLine> > data;

    // para cada set ha um vetor de bits de validade e de alteraçao
    std::vector<std::vector<bool> > valid;
    std::vector<std::vector<bool> > dirty;
    std::vector<std::vector<uint32_t > > counters;

    size_t handleMiss(uint32_t index, uint32_t newaddress);
public:
    HitMissCounter aval;
    Cache(uint32_t cachesize, uint32_t blocksize, uint32_t assoc, ReplacementPolicy & rpolicy, WritePolicy & wpolicy);
    int8_t read(uint32_t address);
    void write(uint32_t address, int8_t value);
};


#endif //CACHESIMULATION_DIRECTMAPCACHE_H
