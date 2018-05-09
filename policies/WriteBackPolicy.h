//
// Created by eduardo on 08/05/18.
//

#ifndef CACHESIMULATION_WRITEBACKPOLICY_H
#define CACHESIMULATION_WRITEBACKPOLICY_H

#include "WritePolicy.h"

class WriteBackPolicy : public WritePolicy {

//protected:
//    Memory memory;
public:
    //explicit WriteBackPolicy(Memory & memory);
    bool write(std::vector<std::pair<int, std::vector<short> > > & set, std::vector<bool> & valid, std::vector<bool> & dirty, int offset, int tag, short value ) override;
    std::vector<short> recoverFromMemory(int address) override;
    void writeToMemory(std::vector<std::pair<int, std::vector<short> > > & data, int idx) override;
};


#endif //CACHESIMULATION_WRITEBACKPOLICY_H
