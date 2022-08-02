#pragma once
#include "MemoryController.h"
#include "LCDDisplayAndKeyboard.h"

//Class to execute program on the EEPROM
class Processor{
private:
    int32_t intRegisters[32];
    float floatRegisters[32];
    bool conditionalFlags[8];
    uint32_t high, low, pc;
    uint32_t programCounter = 0;
    uint32_t* program = nullptr;
    
    MemoryController memUnit;

    void singleStep(uint32_t instruction, uint32_t pc, bool &single);

public:
    void executeProgram(uint32_t* program, bool single);
    
    Processor();
    ~Processor();
};