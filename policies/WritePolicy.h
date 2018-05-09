//
// Created by eduardo on 08/05/18.
//

#ifndef CACHESIMULATION_WRITEPOLICY_H
#define CACHESIMULATION_WRITEPOLICY_H

#include <vector>
#include <utility>
#include <cstdio>

class WritePolicy {
//protected:
//    Memory & memory;
public:
    //virtual explicit WritePolicy(Memory & memory);
    virtual bool write(std::vector<std::pair<int, std::vector<short> > > & set, std::vector<bool> & valid, std::vector<bool> & dirty, int offset, int tag, short data) = 0;
    virtual std::vector<short> recoverFromMemory(int address) = 0;
    virtual void writeToMemory(std::vector<std::pair<int, std::vector<short> > > & data, int idx) = 0;
};

#endif //CACHESIMULATION_WRITEPOLICY_H
