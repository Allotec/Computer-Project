#include "Memory.h"

//Class to execute program on the EEPROM
class Processor{
private:
    uint32_t registers[32];
    uint32_t programCounter = 0;
    uint32_t instructionRegister;

public:
    
    Processor();
    ~Processor();
};