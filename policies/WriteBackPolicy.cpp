//
// Created by eduardo on 08/05/18.
//

#include "WriteBackPolicy.h"

WriteBackPolicy::WriteBackPolicy(std::vector<std::vector<int8_t >> & mem) : memory{mem} {}

bool WriteBackPolicy::write(std::vector<CacheLine> & set, std::vector<bool> & valid,  std::vector<bool> & dirty, size_t offset, uint32_t tag, int8_t value ) {
    // Busca pelo valor em todos os lugares do conjunto associativo
    for (int i = 0; i < set.size(); ++i) {
       if (valid[i] && set[i].tag == tag) {
           set[i].data[offset] = value;
           dirty[i] = true;
           return true;
       }
   }

    // Se nao encontrar o bloco, retorna falso para que a cache substitua algum valor
    return false;
}

std::vector<int8_t> WriteBackPolicy::recoverFromMemory(size_t address) {
    // Encontra a posicao na matriz de memoria onde começa o bloco correspondente
    auto line = address / memory[0].size();
    auto col = address % memory[0].size();
    auto startblock = col - (col % BLOCKSIZE);

    std::vector<int8_t> result;
    for (size_t i = 0; i < BLOCKSIZE; ++i) {
        auto posc = (startblock + i) % memory[0].size();
        auto posl = line;

        // Funciona apenas se o bloco de dados for menor que a quantidade de colunas na memoria
        if (startblock + i >= memory[0].size()) posl = line + 1;

        // Como a memoria nao e multipla do numero de blocos, evita que sejam escritos valores fora da memoria
        if (posl < 100) {
            result.push_back(memory[posl][posc]);
        } else {
            result.push_back(0);
        }
    }

    return result;
}

void WriteBackPolicy::writeToMemory(const CacheLine & block, uint32_t address) {
    auto line = address / memory[0].size();
    auto col = address % memory[0].size();
    auto startblock = col - (col % BLOCKSIZE);

    for (size_t i = 0; i < BLOCKSIZE; ++i) {
        auto posc = (startblock + i) % memory[0].size();
        auto posl = line;

        if (startblock + i >= memory[0].size()) posl = line + 1;

        // Como a memoria nao e multipla do numero de blocos, evita que sejam escritos valores fora da memoria
        if (posl < 100) memory[posl][posc] = block.data[i];
    }

}