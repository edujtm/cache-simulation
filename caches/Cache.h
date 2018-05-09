//
// Created by eduardo on 08/05/18.
//

#ifndef CACHESIMULATION_DIRECTMAPCACHE_H
#define CACHESIMULATION_DIRECTMAPCACHE_H

#include "../policies/ReplacementPolicy.h"
#include "../helper/HitMissCounter.h"
#include "../policies/WritePolicy.h"
#include "../policies/LRUReplace.h"
#include <vector>
#include <bitset>
#include <utility>

#define MAXBIT 1024

class Cache {
    int nsets;
    int cachesize;
    int assoc;
    int blocksize;
    int offsetBitCount;

    ReplacementPolicy * replace;
    WritePolicy * writePolicy;

    // Vetor de sets de blocos de memoria cache
    std::vector<std::vector<std::pair<int, std::vector<short> > > > data;

    // para cada set ha um vetor de bits de validade e de alteraçao
    std::vector<std::vector<bool> > valid;
    std::vector<std::vector<bool> > dirty;
    std::vector<std::vector<int> > counters;

public:
    HitMissCounter aval;
    Cache(int cachesize, int blocksize, int assoc, ReplacementPolicy & rpolicy, WritePolicy & wpolicy);
    short read(int address);
    void write(int address, short value);
};


#endif //CACHESIMULATION_DIRECTMAPCACHE_H
