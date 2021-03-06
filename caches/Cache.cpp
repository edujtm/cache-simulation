//
// Created by eduardo on 08/05/18.
//

#include "Cache.h"

Cache::Cache(uint32_t cchsize, uint32_t blksize, uint32_t associativity, ReplacementPolicy & rpolicy, WritePolicy & wpolicy) {

    replace = &rpolicy;
    nsets = cchsize / (blksize * associativity);
    writePolicy = &wpolicy;

    assoc = associativity;
    blocksize = blksize;
    cachesize = cchsize;

    aval = HitMissCounter();

    // Quantidade de bits que representam o indice e o deslocamento de bloco
    offsetBitCount = log(blocksize) / log(2);
    indexBitCount = log(nsets) / log(2) + offsetBitCount;

    // A linha abaixo inicializa toda a cache, ficou complexa pois ela da suporte aos 3 tipos de cache
    data = std::vector<std::vector<CacheLine> >(nsets, std::vector<CacheLine>(assoc, CacheLine()));
    dirty = std::vector<std::vector<bool>>(nsets, std::vector<bool>(assoc, false));
    valid = std::vector<std::vector<bool>>(nsets, std::vector<bool>(assoc, false));
    counters = std::vector<std::vector<uint32_t >>(nsets, std::vector<uint32_t >(assoc, 0));
}

int8_t Cache::read(uint32_t address) {
    // Obtendo valores da cache atraves do endereço
    auto offset = address % blocksize;
    auto index = (address >> offsetBitCount) % nsets;
    auto tag = address >> indexBitCount;

    aval.readTotal++;

    // Valor de retorno da leitura
    int8_t value = 0;

    // Busca pela tag nos valores do conjunto associativo
    bool hit = false;
    for (size_t i = 0; i < assoc; ++i) {
        if(valid[index][i] && data[index][i].tag == tag) {
            aval.readHits++;
            value = data[index][i].data[offset];
            replace->update(counters[index], i);
            hit = true;
            break;
        }
    }

    // Se nao encontrou a tag, gerencia o movimento de dados e busca o dado novamente
    if (!hit) {
        aval.readMisses++;
        size_t replacedidx = handleMiss(index, address);
        value = data[index][replacedidx].data[offset];
    }

    return value;
}

void Cache::write(uint32_t address, int8_t value) {
    auto offset = address % blocksize;
    auto index = (address >> offsetBitCount) % nsets;
    auto tag =  address >> indexBitCount;

    aval.writeTotal++;

    bool ableToWrite = writePolicy->write(data[index], valid[index], dirty[index], offset, tag, value);

    if (ableToWrite) aval.writeHits++;

    while (!ableToWrite) {
        aval.writeMisses++;
        handleMiss(index, address);
        ableToWrite = writePolicy->write(data[index], valid[index], dirty[index], offset, tag, value);
    }

}

size_t Cache::handleMiss(uint32_t index, uint32_t newaddress) {
    // Busca um bloco de memoria para substituir
    size_t blockidx = replace->getBlockIndex(valid[index], counters[index]);

    uint32_t tag = newaddress >> indexBitCount;

    // Reconstroi o endereco do bloco a ser substituido
    uint32_t oldaddress = (data[index][blockidx].tag << indexBitCount) | (index << offsetBitCount);

    if (valid[index][blockidx]) writePolicy->writeToMemory(data[index][blockidx], oldaddress);

    auto block = writePolicy->recoverFromMemory(newaddress);

    data[index][blockidx].tag = tag;
    data[index][blockidx].data = block;
    valid[index][blockidx] = true;
    dirty[index][blockidx] = false;
    return blockidx;
}