//
// Created by eduardo on 08/05/18.
//

#include "LRUReplace.h"

int LRUReplace::getBlockIndex(std::vector<bool> &valid, std::vector<int> & counters) {
    for (int i = 0; i < valid.size(); ++i) {
        if (!valid[i]) {
            update(counters, i);
            return i;
        }
    }

    int replace = 0;
    for (int i = 1; i < counters.size(); ++i) {
        if (counters[i] < counters[i-1]) {
            replace = i;
        }
    }
    update(counters, replace);
    return replace;
}

void LRUReplace::update(std::vector<int> & counters, int idx) {
    counters[idx] = now++;
}