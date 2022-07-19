#pragma once
#include "Memory.h"

union fDouble{
    double num;
    float f1;
    uint64_t bits;
};

//Class to execute program on the EEPROM
class Processor{
private:
    int32_t intRegisters[32];
    float floatRegisters[32];
    bool conditionalFlags[8];
    uint32_t high, low, pc;
    uint32_t programCounter = 0;
    uint32_t* program = nullptr;

public:
    void executeProgram(uint32_t* program);
    
    Processor();
    ~Processor();
};