#pragma once
#include <iostream>
using namespace std;

class MemoryModule {
    unsigned char m[0x0F00];
    bool active;


public:
    MemoryModule() {
        for (int i = 0; i < 0x0F00; i++)
            m[i] = 0;

        active = false;
    }

    unsigned char read(unsigned short add) {

        active = true;
        return m[add];
    }

    void write(unsigned short add, unsigned char v) {
        active = true;

        m[add] = v;
    }

    void loadRawBinary(unsigned short add, unsigned short word) {
        if (add + 1 < 0x0F00) {
            m[add] = (word >> 8) & 0xFF;

            m[add + 1] = word & 0xFF;
        }
    }

    double getPow() {
        double draw;
        if (active)
            draw = 0.5;
        else
            draw = 0.1;
        active = false;
        return draw;

    }
};