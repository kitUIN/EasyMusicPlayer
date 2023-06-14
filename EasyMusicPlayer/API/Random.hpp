#pragma once

#include<time.h>

class Random
{
public:
    Random(void)
    {
        seed = time(NULL) % 10000;
    }
    int operator()(void)
    {
        int temp = seed;
        seed = seed * seed;
        while (seed == temp) {
            temp = seed = time(NULL) % 10000;
            seed = seed * seed;
        }
        seed /= 100;
        seed %= 10000;
        return seed;
    }
private:
    int seed;
};
