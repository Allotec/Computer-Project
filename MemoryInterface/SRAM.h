#include "Memory.h"

//15 Address pins
//8 Data pins
//Active High chip Enable (Using an Inverter IC)

/*
CE  OE  WE  I/O     Function
H   L   H   DATAOUT Read Data
H   X   L   DATAIN  Write Data
H   H   H   High-Z  Outputs Disabled
*/

//Used to store data for each program
class SRAM{
private:
    void writeAddress(uint16_t address, uint8_t data); //actual address are only 15 bits so check for that
    uint8_t readAddress(uint16_t address);

public:
    const int size = 0x7FFF;
    uint32_t fetchData(uint16_t address);
    void writeData(uint16_t address, uint32_t data);

    SRAM();
    ~SRAM();
};
