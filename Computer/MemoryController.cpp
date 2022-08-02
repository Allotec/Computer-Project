#include "MemoryController.h"

//SRAM Memory Controller
//Constructor 
//Defaults to 1 sram
MemoryController::MemoryController(){
    sram = new Memory[1];
    sram[0].setMemAddr(0);
    this->sramNum = 1;
    this->maxAddress = ((uint32_t)size * (uint32_t)(sramNum + 1) - 1);
}

MemoryController::MemoryController(uint8_t sramNum){
    this->sramNum = sramNum;
    this->maxAddress = ((uint32_t)size * (uint32_t)(sramNum + 1) - 1);
    sram = new Memory[sramNum];

    for(int i = 0; i < sramNum; i++){
        sram[i].setMemAddr(i);
    }
}

//Destructor
MemoryController::~MemoryController(){
    delete [] sram;
}

//Getter
uint8_t MemoryController::getSramNum(){
    return(sramNum);
}

uint32_t MemoryController::getMaxAddress(){
    return(maxAddress);
}

//SRAM Memory writes
//Only have to check if its in the address space because its a byte write
void MemoryController::writeAddress(uint32_t address, uint8_t data){
    if(address > this->maxAddress)
        return;

    sram[address / size].writeAddress(address - ((address / size) * size), data);
}

//have to check if its inside address space and the address is divisible by two
void MemoryController::writeAddress(uint32_t address, uint16_t data){
    if(address > this->maxAddress || address % 2 != 0)
        return;

    sram[address / size].writeAddress(address - ((address / size) * size), ((data & 0xFF00) >> 8));
    sram[address / size].writeAddress(address - ((address / size) * size) + 1, (data & 0x00FF));
}

void MemoryController::writeAddress(uint32_t address, uint32_t data){
    if(address > this->maxAddress || address % 4 != 0)
        return;

    sram[address / size].writeAddress(address - ((address / size) * size), ((data & 0xFF000000) >> 24));
    sram[address / size].writeAddress(address - ((address / size) * size) + 1, ((data & 0x00FF0000) >> 16));
    sram[address / size].writeAddress(address - ((address / size) * size) + 2, ((data & 0x0000FF00) >> 8));
    sram[address / size].writeAddress(address - ((address / size) * size) + 3, (data & 0x000000FF));
}

void MemoryController::writeAddress(uint32_t address, float data){
    if(address > this->maxAddress || address % 4 != 0)
        return;

    union fDouble floatData;
    floatData.f1 = data;

    sram[address / size].writeAddress(address - ((address / size) * size), ((floatData.bits & 0xFF000000) >> 24));
    sram[address / size].writeAddress(address - ((address / size) * size) + 1, ((floatData.bits & 0x00FF0000) >> 16));
    sram[address / size].writeAddress(address - ((address / size) * size) + 2, ((floatData.bits & 0x0000FF00) >> 8));
    sram[address / size].writeAddress(address - ((address / size) * size) + 3, (floatData.bits & 0x000000FF));
}

//Stores a double in memory
void MemoryController::writeAddress(uint32_t address, double data){
    if(address > this->maxAddress || address % 8 != 0)
        return;

    union fDouble floatData;
    floatData.num = data;

    sram[address / size].writeAddress(address - ((address / size) * size), ((floatData.bits & 0xFF00000000000000) >> 56));
    sram[address / size].writeAddress(address - ((address / size) * size) + 1, ((floatData.bits & 0x00FF000000000000) >> 48));
    sram[address / size].writeAddress(address - ((address / size) * size) + 2, ((floatData.bits & 0x0000FF0000000000) >> 40));
    sram[address / size].writeAddress(address - ((address / size) * size) + 3, ((floatData.bits & 0x000000FF00000000) >> 32));
    sram[address / size].writeAddress(address - ((address / size) * size) + 4, ((floatData.bits & 0x00000000FF000000) >> 24));
    sram[address / size].writeAddress(address - ((address / size) * size) + 5, ((floatData.bits & 0x0000000000FF0000) >> 16));
    sram[address / size].writeAddress(address - ((address / size) * size) + 6, ((floatData.bits & 0x000000000000FF00) >> 8));
    sram[address / size].writeAddress(address - ((address / size) * size) + 7, (floatData.bits & 0x00000000000000FF));
}

//SRAM memory reads
uint8_t MemoryController::readByte(uint32_t address){
    if(address > this->maxAddress){
        Serial.print("Returning bc "); Serial.print(address); Serial.print(" > "); Serial.println(((uint32_t)size * (uint32_t)(sramNum + 1)));
        return(0);
    }

    Serial.print("Chip- "); Serial.print(address / size); Serial.print(", "); Serial.print("Address- "); Serial.println(address - ((address / size) * size));

    return(sram[address / size].readAddress(address - ((address / size) * size)));
}

uint16_t MemoryController::readHalfWord(uint32_t address){
    if(address > this->maxAddress || address % 2 != 0)
        return(0);

    uint16_t halfWord = 0;

    halfWord = ((uint16_t)sram[address / size].readAddress(address - ((address / size) * size))) << 8;
    halfWord |= ((uint16_t)sram[address / size].readAddress(address - ((address / size) * size) + 1)) & 0x00FF;

    return(halfWord);
}

uint32_t MemoryController::readWord(uint32_t address){
    if(address > this->maxAddress || address % 4 != 0)
        return(0);

    uint32_t word = 0;

    word = (((uint32_t)sram[address / size].readAddress(address - ((address / size) * size))) << 24) & 0xFF000000;
    word |= (((uint32_t)sram[address / size].readAddress(address - ((address / size) * size) + 1)) << 16) & 0x00FF0000;
    word |= (((uint32_t)sram[address / size].readAddress(address - ((address / size) * size) + 2)) << 8) & 0x0000FF00;
    word |= ((uint32_t)sram[address / size].readAddress(address - ((address / size) * size) + 3)) & 0x000000FF;

    return(word);
}

float MemoryController::readFloat(uint32_t address){
    if(address > this->maxAddress || address % 4 != 0)
        return(0);

    fDouble word;

    word.bits = (((uint32_t)sram[address / size].readAddress(address - ((address / size) * size))) << 24) & 0xFF000000;
    word.bits |= (((uint32_t)sram[address / size].readAddress(address - ((address / size) * size) + 1)) << 16) & 0x00FF0000;
    word.bits |= (((uint32_t)sram[address / size].readAddress(address - ((address / size) * size) + 2)) << 8) & 0x0000FF00;
    word.bits |= ((uint32_t)sram[address / size].readAddress(address - ((address / size) * size) + 3)) & 0x000000FF;

    return(word.f1);
}

double MemoryController::readDouble(uint32_t address){
    if(address > this->maxAddress || address % 8 != 0)
        return(0);

    fDouble word;

    word.bits = (((uint64_t)sram[address / size].readAddress(address - ((address / size) * size))) << 56) & 0xFF00000000000000;
    word.bits |= (((uint64_t)sram[address / size].readAddress(address - ((address / size) * size) + 1)) << 48) & 0x00FF000000000000;
    word.bits |= (((uint64_t)sram[address / size].readAddress(address - ((address / size) * size) + 2)) << 40) & 0x0000FF0000000000;
    word.bits |= (((uint64_t)sram[address / size].readAddress(address - ((address / size) * size) + 3)) << 32) & 0x000000FF00000000;
    word.bits |= (((uint64_t)sram[address / size].readAddress(address - ((address / size) * size) + 4)) << 24) & 0x00000000FF000000;
    word.bits |= (((uint64_t)sram[address / size].readAddress(address - ((address / size) * size) + 5)) << 16) & 0x0000000000FF0000;
    word.bits |= (((uint64_t)sram[address / size].readAddress(address - ((address / size) * size) + 6)) << 8) & 0x000000000000FF00;
    word.bits |= (((uint64_t)sram[address / size].readAddress(address - ((address / size) * size) + 7))) & 0x00000000000000FF;

    return(word.num);
}

