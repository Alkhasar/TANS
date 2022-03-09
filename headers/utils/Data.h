#ifndef DATA
#define DATA

#include <cinttypes>

struct Data{
    // 32 bits for event number
    uint32_t event;
    
    // 8 bits for detector
    uint8_t detector;

    // 64 bit for time measure
    double time;
};

#endif