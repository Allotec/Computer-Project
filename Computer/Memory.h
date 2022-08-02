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

const uint32_t size = 0x8000;

const uint8_t StaticData = 0;
const uint8_t DynamicData = 1;
const uint8_t Stack = 2;
const uint8_t EEPROM0 = 3;
const uint8_t EEPROM1 = 4;

//Unions to be able to do bitwise operations on a float or a double
union fDouble{
    double num;
    float f1;
    uint64_t bits;
};

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

    Memory(uint8_t MEMNum);
    Memory();
    ~Memory();
};

