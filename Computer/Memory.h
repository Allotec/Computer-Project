//Constant Defintions
#pragma once
#include <stdint.h>
#include "Arduino.h"

const uint8_t addressPinsNum = 15;

const uint8_t addressPins[addressPinsNum] = {
    22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36
};

const uint8_t dataPinsNum = 8;

const uint8_t dataPins[dataPinsNum] = {
    42, 43, 44, 50, 46, 47, 48, 49
};

const uint8_t chipEnableSize = 3;

const uint8_t chipEnable[chipEnableSize] = {37, 38, 39};
const uint8_t outputEnable = 40;
const uint8_t writeEnable = 41;

const int size = 0x7FFF;

const uint8_t StaticData = 0;
const uint8_t DynamicData = 1;
const uint8_t Stack = 2;
const uint8_t EEPROM0 = 3;
const uint8_t EEPROM1 = 4;

/*
MIPS Memory Layout (32 Bit)
Text- 0x0040 0000 to 0x1000 0000
Static Data- 0x1000 0000 to 0x1000 8000
Dynamic Data- 0x1000 8000 growing to 0x7FFF FFFC
Stack- 0x7FFF FFFC to 0x1000 8000

Memory Mapping (16 Bit)
Text- Subtract 0x0040 0000 from the address and add the offset from where the program starts (In the EEPROMS). If the text goes over 1 EEPROMS space go to next.
Static Data- Subtract base address put in SRAM 1.
Dynamic Data- Subtract base address put in SRAM 2.
Stack- Subtract base address put in SRAM 3 but grow upwards by turning negative numbers into positive.

*/

class Memory{
private:
    uint8_t MEMAddr;

public:
    void init();
    void setMemAddr(uint8_t MEMAddr);
    void writeAddress(uint16_t address, uint8_t data); 
    uint8_t readAddress(uint16_t address);
    uint32_t fetchData(uint16_t address); 
    void writeData(uint16_t address, uint32_t data);
    void test();
    //void writeProgram(uint32_t program[]);

    Memory(uint8_t MEMNum);
    ~Memory();
};
