#include "caches/Cache.h"
#include "policies/LRUReplace.h"
#include "policies/WriteBackPolicy.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <random>
#include <functional>
#include <fstream>

#define MEMLINE 100
#define MEMCOL 1000
#define CACHESIZE 1024u
#define BLOCKSIZE 16u
#define DIRECT 1u
#define SETASSOC 8
#define FULLASSOC 64

void runCache(uint32_t size, uint32_t blksize, uint32_t assoc, std::fstream & file) {
    // Gerador de numeros aleatorios
    std::default_random_engine generator;
    std::uniform_int_distribution<int8_t> distribution(-128, 127);
    auto memvalue = std::bind(distribution, generator);

    // Inicia a memoria com valores aleatorios entre -128 e 127
    std::vector<std::vector<int8_t > > memory (MEMLINE, std::vector<int8_t >(MEMCOL, 0));
    for (size_t i = 0; i < MEMLINE; ++i) {
        for (size_t j = 0; j < MEMCOL; ++j) {
            memory[i][j] = memvalue();
        }
    }

    // LRUReplace indica qual o bloco vai ser substituido na cache
    LRUReplace replace = LRUReplace();

    // WriteBackPolicy e uma especie de protocolo de comunicaçao entre a memoria e a cache
    WriteBackPolicy WB = WriteBackPolicy(memory);
    Cache cache = Cache(size, blksize, assoc, replace, WB);

    file << "Iterando pelas colunas" << std::endl;
    for (uint32_t row = 0; row < MEMLINE; row += 1) {
        for (uint32_t col = 0; col < MEMCOL; col += 1) {
            uint32_t address = col + row * MEMCOL;
            cache.read(address);
        }
    }

    file << cache.aval << std::endl;

    file << "Iterando pelas linhas" << std::endl;
    for (uint32_t col = 0; col < MEMCOL; col += 1) {
        for (uint32_t row = 0; row < MEMLINE; row += 1) {
            uint32_t address = col + row * MEMCOL;
            cache.read(address);
        }
    }
    file << cache.aval << std::endl;
}

int main() {

    std::cout << "Inicializando execuçao das caches." << std::endl;
    std::cout << "Tamanho dos tipos de variaveis utilizados: " << std::endl;
    std::cout << "Tamanho de uint32_t em bytes: " << sizeof(uint32_t) << " bytes." << std::endl;
    std::cout << "Tamanho de int8_t em bytes: "<< sizeof(int8_t) << " bytes." << std::endl;
    std::cout << "Tamanho de size_t em bytes: " << sizeof(size_t) << " bytes" << std::endl << std::endl;

    pid_t directpid = fork();

    if (directpid < 0) {
        std::cout << "Erro ao criar processo filho para cache direta." << std::endl;
        return 1;
    }

    if (directpid > 0) {
        // processo pai

        pid_t setpid = fork();

        if (setpid < 0) {
            std::cout << "Erro ao criar processo filho para cache associativa em conjunto" << std::endl;
            return 1;
        }

        if (setpid > 0) {
            // processo pai

            pid_t assocpid = fork();

            if (assocpid < 0) {
                std::cout << "Erro ao criar processo filho para cache totalmente associativa" << std::endl;
                return 1;
            }

            if (assocpid > 0) {
                // processo pai
                int statusdirect, statusset, statusassoc;
                waitpid(directpid, &statusdirect, 0);
                waitpid(setpid, &statusset, 0);
                waitpid(assocpid, &statusassoc, 0);

                std::cout << "Finalizando execuçao do processo pai..." << std::endl;
            } else {
                // processo filho para a cache totalmente associativa

                std::fstream filefullassoc;
                filefullassoc.open("/home/eduardo/CLionProjects/CacheSimulation/results/fullassoc.log", std::fstream::in | std::fstream::out| std::fstream::trunc);

                if (!filefullassoc.is_open()) {
                    std::cout << "" << std::endl;
                    std::cout << "Erro ao abrir o arquivo" << std::endl;
                    return 1;
                }

                filefullassoc << "Resultado da cache com mapeamento totalmente associativo:" << std::endl;

                runCache(CACHESIZE, BLOCKSIZE, FULLASSOC, filefullassoc);

                filefullassoc.close();
            }
        } else {
            // processo filho para a cache associativa em conjunto
            std::fstream filesetassoc;
            filesetassoc.open("/home/eduardo/CLionProjects/CacheSimulation/results/setassoc.log", std::fstream::in | std::fstream::out| std::fstream::trunc);

            if (!filesetassoc.is_open()) {
                std::cout << "" << std::endl;
                std::cout << "Erro ao abrir o arquivo" << std::endl;
                return 1;
            }

            filesetassoc << "Resultado da cache com mapeamento em conjunto:" << std::endl;
            runCache(CACHESIZE, BLOCKSIZE, SETASSOC, filesetassoc);
            filesetassoc.close();
        }
    } else {
        // processo filho para a cache direta

        std::fstream filedirect;
        filedirect.open("/home/eduardo/CLionProjects/CacheSimulation/results/direct.log", std::fstream::in | std::fstream::out| std::fstream::trunc);

        if (!filedirect.is_open()) {
            std::cout << "" << std::endl;
            std::cout << "Erro ao abrir o arquivo" << std::endl;
            return 1;
        }

        filedirect << "Resultado da cache com mapeamento direto:" << std::endl;
        runCache(CACHESIZE, BLOCKSIZE, DIRECT, filedirect);
        filedirect.close();
    }
    return 0;
}