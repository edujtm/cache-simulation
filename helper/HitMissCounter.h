//
// Created by eduardo on 08/05/18.
//

#ifndef CACHESIMULATION_HITMISSCOUNTER_H
#define CACHESIMULATION_HITMISSCOUNTER_H

#include <iostream>
#include <iomanip>

struct HitMissCounter {
    int readHits;
    int readTotal;
    int readMisses;

    int writeHits;
    int writeTotal;
    int writeMisses;

    HitMissCounter() {
        readHits = 0;
        readTotal = 0;
        readMisses = 0;

        writeHits = 0;
        writeTotal = 0;
        writeMisses = 0;
    }

    double getHitRate() const {
        return (double) (readHits + writeHits) / (writeTotal + readTotal > 0 ? writeTotal + readTotal : 1);
    }

    double getMissRate() const {
        return (double) (readMisses + writeMisses) / (writeTotal + readTotal > 0 ? writeTotal + readTotal : 1);
    }

    friend std::ostream & operator<<(std::ostream & stdout, const HitMissCounter & value) {
        std::cout << std::fixed;
        stdout << "Resultados: " << std::endl;
        stdout << "Hit rate (hits/total): " << std::setprecision(3) << value.getHitRate() << std::endl;
        stdout << "Miss rate (misses/total): " << std::setprecision(3) << value.getMissRate() << std::endl;
        return stdout;
    }
};



#endif //CACHESIMULATION_HITMISSCOUNTER_H
