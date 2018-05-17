//
// Created by eduardo on 08/05/18.
//

#include "LRUReplace.h"

LRUReplace::LRUReplace() {
    now = 0u;
}

size_t LRUReplace::getBlockIndex(std::vector<bool> &valid, std::vector<uint32_t> & counters) {
    for (size_t i = 0; i < valid.size(); ++i) {
        if (!valid[i]) {
            update(counters, i);
            return i;
        }
    }

    size_t replace = 0;
    for (size_t i = 1; i < counters.size(); ++i) {
        if (counters[i] < counters[i-1]) {
            replace = i;
        }
    }
    update(counters, replace);
    return replace;
}

void LRUReplace::update(std::vector<uint32_t > & counters, size_t idx) {
    counters[idx] = now++;
}