#include "Memory.h"

//15 Address pins
//8 Data pins
//Active Low chip Enable
/*
CE  OE  WE  I/O     Function
L   L   H   DATAOUT Read Data
L   X   L   DATAIN  Write Data
L   H   H   High-Z  Outputs Disabled
*/

//Used to store the program instructions
class HEEPROM{
private:
    void writeAddress(uint16_t address, uint8_t data); //actual address are only 15 bits so check for that
    uint8_t readAddress(uint16_t address);
    
public:
    uint32_t fetchInstruction(uint16_t address);
    void writeProgram(uint32_t program[]);

    HEEPROM();
    ~HEEPROM();
};
