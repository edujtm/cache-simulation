//
// Created by eduardo on 08/05/18.
//

#include "WriteBackPolicy.h"

WriteBackPolicy::WriteBackPolicy(std::vector<std::vector<int8_t >> & mem) : memory{mem} {}

bool WriteBackPolicy::write(std::vector<CacheLine> & set, std::vector<bool> & valid,  std::vector<bool> & dirty, size_t offset, uint32_t tag, int8_t value ) {
    // Busca pelo valor em todos os lugares do conjunto associativo
    for (int i = 0; i < set.size(); ++i) {
       if (set[i].tag == tag) {
           set[i].data[offset] = value;
           dirty[i] = true;
           return true;
       }
   }

    // Se nao encontrar lugar vazio, retorna falso para que a cache substitua algum valor
    return false;
}

std::vector<int8_t> WriteBackPolicy::recoverFromMemory(size_t address) {
    // Encontra a posicao na matriz de memoria onde começa o bloco correspondente
    auto line = address / memory[0].size();
    auto col = address % memory[0].size();

    std::vector<int8_t> result;
    for (size_t i = 0; i < BLOCKSIZE; ++i) {
        auto posc = (col+i) % memory.size();
        auto posl = line;

        // Funciona apenas se o bloco de dados for menor que a quantidade de colunas na memoria
        if (col + i >= memory.size()) posl++;
        result.push_back(memory[posl][posc]);
    }

    return result;
}

void WriteBackPolicy::writeToMemory(const CacheLine & block, uint32_t address) {
    auto line = address / memory[0].size();
    auto col = address % memory[0].size();

    for (size_t i = 0; i < BLOCKSIZE; ++i) {
        auto posc = (col + i) % memory.size();
        auto posl = line;

        if (col + i >= memory.size()) posl++;
        memory[posl][posc] = block.data[i];
    }

}