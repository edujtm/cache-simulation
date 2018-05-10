//
// Created by eduardo on 08/05/18.
//

#include "Cache.h"

Cache::Cache(int cchsize, int blksize, int associativity, ReplacementPolicy & rpolicy, WritePolicy & wpolicy) {

    replace = &rpolicy;
    // TODO verificar se o calculo abaixo esta funcionando (se ha algum erro de conversao de tipos)
    nsets = cchsize / blksize * associativity;
    writePolicy = &wpolicy;

    assoc = associativity;
    blocksize = blksize;
    cachesize = cchsize;

    aval = HitMissCounter();

    // A linha abaixo inicializa toda a cache, ficou complexa pois ela da suporte aos 3 tipos de cache
    data = std::vector<std::vector<std::pair<int, std::vector<short> > > >(nsets, std::vector<std::pair<int, std::vector<short> > >(assoc, std::make_pair(0, std::vector<short>(blocksize, 0))));
    dirty = std::vector<std::vector<bool>>(nsets, std::vector<bool>(assoc, false));
    valid = std::vector<std::vector<bool>>(nsets, std::vector<bool>(assoc, false));
    counters = std::vector<std::vector<int>>(nsets, std::vector<int>(assoc, 0));
}

short Cache::read(int address) {
    // Obtendo valores da cache atraves do endereço
    auto offset = address % blocksize;
    auto index = (address >> 3) % nsets;
    auto tag = address >> 10;

    aval.readTotal++;

    // Valor de retorno da leitura
    short value = 0;

    // Busca pela tag nos valores do conjunto associativo
    bool hit = false;
    for (int i = 0; i < assoc; ++i) {
        if(valid[index][i] && data[index][i].first == tag) {
            aval.readHits++;
            //value = data[index][i].second[offset];
            replace->update(counters[index], i);
            hit = true;
            break;
        }
    }

    if (!hit) {
        int blockidx = replace->getBlockIndex(valid[index], counters[index]);
        aval.readMisses++;
        // TODO recuperar bloco da memoria e substituir em data[index][blockidx] (data[index][blockidx].first = tag, data[index][blockidx].second = block)
        data[index][blockidx].first = tag;
        valid[index][blockidx] = true;
        value = 0;
    }

    return value;
}

void Cache::write(int address, short value) {
    auto offset = address % blocksize;
    auto index = (address >> 3) % nsets;
    auto tag =  address >> 10;

    aval.writeTotal++;

    /*
    bool ableToWrite = writePolicy.write(data[index], valid[index], dirty[index], offset, tag, value);
    while(!ableToWrite) {
        // TODO enviar valor antigo para memoria, sobrescrever o novo bloco vazio com o novo valor
        int blockidx = replace.getBlockIndex(valid[index], counters[index]);
        writePolicy.writeToMemory(data[index], blockidx);
        ableToWrite = writePolicy.write(data[index], valid[index], dirty[index], offset, tag, value);
    }
    */

    // TODO remover esse codigo temporario, funciona apenas com as tags, sem movimentaçao de dados
    bool hit = false;
    for (int i = 0; i < assoc; ++i) {
        if (data[index][i].first == tag) {
            aval.writeHits++;
            replace->update(counters[index], i);
            hit = true;
        }
    }

    if (!hit) {
        int blockidx = replace->getBlockIndex(valid[index], counters[index]);
        aval.writeMisses++;
        data[index][blockidx].first = tag;
        valid[index][blockidx] = true;
    }
}