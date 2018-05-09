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


void runDirectCache() {
    LRUReplace replace = LRUReplace();
    WriteBackPolicy WB = WriteBackPolicy();
    Cache directcache = Cache(CACHESIZE, BLOCKSIZE, DIRECT, replace, WB);

    std::cout << "Iterando pelas colunas" << std::endl;
    for (int row = 0; row < 100; row += 2) {
        for (int col = 0; col < 1000; col += 2) {
            int address = col + row * 100;
            directcache.read(address);
        }
    }

    std::cout << directcache.aval;

    std::cout << "Iterando pelas linhas" << std::endl;
    for (int col = 0; col < 1000; col += 2) {
        for (int row = 0; row < 100; row += 2) {
            int address = col + row * 100;
            directcache.read(address);
        }
    }

    std::cout << directcache.aval;
}

int main() {

    runDirectCache();
    /*
    pid_t result = fork();

    if (result < 0) {
        std::cout << "Erro ao criar processo filho na main." << std::endl;
        return 1;
    }

    if (result == 0) {
        int status;
        waitpid();
    } else {

    }
    */
    return 0;
}