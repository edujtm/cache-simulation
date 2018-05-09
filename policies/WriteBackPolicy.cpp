//
// Created by eduardo on 08/05/18.
//

#include "WriteBackPolicy.h"

bool WriteBackPolicy::write(std::vector<std::pair<int, std::vector<short> > > & set, std::vector<bool> & valid,  std::vector<bool> & dirty, int offset, int tag, short value ) {
    // Busca pelo valor em todos os lugares do conjunto associativo
    for (int i = 0; i < set.size(); ++i) {
       if (set[i].first == tag) {
           set[i].second[offset] = value;
           dirty[i] = true;
           return true;
       }
   }

    // Se nao encontrar o valor, busca um local vazio
    for (int i = 0; i < valid.size(); ++i) {
        if (!valid[i]) {
            set[i].first = tag;
            set[i].second[offset] = value;
            dirty[i] = true;
            return true;
        }
    }

    // Se nao encontrar lugar vazio, retorna falso para que a cache substitua algum valor
    return false;
}

std::vector<short> WriteBackPolicy::recoverFromMemory(int address) {

}

void WriteBackPolicy::writeToMemory(std::vector<std::pair<int, std::vector<short> > > & data, int idx) {

}