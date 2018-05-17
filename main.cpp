#include "caches/Cache.h"
#include "policies/LRUReplace.h"
#include "policies/WriteBackPolicy.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <random>
#include <functional>

#define MEMLINE 100
#define MEMCOL 1000
#define CACHESIZE 1024u
#define BLOCKSIZE 16u
#define DIRECT 1u
#define SETASSOC 8
#define FULLASSOC 64

void runCache(uint32_t size, uint32_t blksize, uint32_t assoc) {
    // Gerador de numeros aleatorios
    std::default_random_engine generator;
    std::uniform_int_distribution<int8_t> distribution(-128, 127);
    auto memvalue = std::bind(distribution, generator);

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

    std::cout << "Iterando pelas colunas" << std::endl;
    for (uint32_t row = 0; row < 100; row += 1) {
        for (uint32_t col = 0; col < 1000; col += 1) {
            uint32_t address = col + row * 100;
            cache.read(address);
        }
    }

    std::cout << cache.aval;

    std::cout << "Iterando pelas linhas" << std::endl;
    for (uint32_t col = 0; col < 1000; col += 1) {
        for (uint32_t row = 0; row < 100; row += 1) {
            uint32_t address = col + row * 100;
            cache.read(address);
        }
    }
    std::cout << cache.aval;
}

int main() {

    std::cout << "Inicializando execuçao das caches." << std::endl;
    std::cout << "Tamanho dos tipos de variaveis utilizados: " << std::endl;
    std::cout << "Tamanho de uint32_t em bytes: " << sizeof(uint32_t) << " bytes." << std::endl;
    std::cout << "Tamanho de int8_t em bytes: "<< sizeof(int8_t) << " bytes." << std::endl;
    std::cout << "Tamanho de size_t em bytes: " << sizeof(size_t) << " bytes" << std::endl << std::endl;

    runCache(CACHESIZE, BLOCKSIZE, DIRECT);
    /*
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
                runCache(CACHESIZE, BLOCKSIZE, FULLASSOC);
            }
        } else {
            // processo filho para a cache associativa em conjunto
            runCache(CACHESIZE, BLOCKSIZE, SETASSOC);
        }
    } else {
        // processo filho para a cache direta
        runCache(CACHESIZE, BLOCKSIZE, DIRECT);
    }
     */
    return 0;
}