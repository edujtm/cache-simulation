#include <iostream>

#define CACHESIZE 1024
#define BLOCKSIZE 8
#define DIRECT 1
#define SETASSOC 8
#define FULLASSOC 64

#include "caches/Cache.h"
#include "policies/LRUReplace.h"
#include "policies/WriteBackPolicy.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>


void runCache(int size, int blksize, int assoc) {
    LRUReplace replace = LRUReplace();
    WriteBackPolicy WB = WriteBackPolicy();
    Cache cache = Cache(size, blksize, assoc, replace, WB);

    std::cout << "Iterando pelas colunas" << std::endl;
    for (int row = 0; row < 100; row += 2) {
        for (int col = 0; col < 1000; col += 2) {
            int address = col + row * 100;
            cache.read(address);
        }
    }

    std::cout << cache.aval;

    std::cout << "Iterando pelas linhas" << std::endl;
    for (int col = 0; col < 1000; col += 2) {
        for (int row = 0; row < 100; row += 2) {
            int address = col + row * 100;
            cache.read(address);
        }
    }
    // TODO reiniciar aval
    std::cout << cache.aval;
}

int main() {

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
    return 0;
}