#include "MemoryController.h"

//SRAM Memory Controller
//Constructor 
//Defaults to 1 sram
MemoryController::MemoryController(){
    sram = new Memory[1];
    sram[0].setMemAddr(0);
}

MemoryController::MemoryController(uint8_t sramNum){
    sram = new Memory[sramNum];

    for(int i = 0; i < sramNum; i++){
        sram[i].setMemAddr(i);
    }
}

//Destructor
MemoryController::~MemoryController(){
    delete [] sram;
}

//SRAM Memory writes
//Only have to check if its in the address space because its a byte write
void MemoryController::writeAddress(uint32_t address, uint8_t data){
    if(address > size * sramNum)
        return;

    sram[address / size].writeAddress(address - ((address / size) * size), data);
}

//have to check if its inside address space and the address is divisible by two
void MemoryController::writeAddress(uint32_t address, uint16_t data){
    if(address > size * sramNum || address % 2 != 0)
        return;

    sram[address / size].writeAddress(address - ((address / size) * size), ((data & 0xFF00) >> 8));
    sram[address / size].writeAddress(address - ((address / size) * size) + 1, (data & 0x00FF));
}

void MemoryController::writeAddress(uint32_t address, uint32_t data){
    if(address > size * sramNum || address % 4 != 0)
        return;

    sram[address / size].writeAddress(address - ((address / size) * size), ((data & 0xFF000000) >> 24));
    sram[address / size].writeAddress(address - ((address / size) * size) + 1, ((data & 0x00FF0000) >> 16));
    sram[address / size].writeAddress(address - ((address / size) * size) + 2, ((data & 0x0000FF00) >> 8));
    sram[address / size].writeAddress(address - ((address / size) * size) + 3, (data & 0x000000FF));
}

void MemoryController::writeAddress(uint32_t address, float data){
    if(address > size * sramNum || address % 4 != 0)
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
    if(address > size * sramNum || address % 8 != 0)
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

}

uint16_t MemoryController::readHalfWord(uint32_t address){

}

uint32_t MemoryController::readWord(uint32_t address){

}

float MemoryController::readFloat(uint32_t address){

}

double MemoryController::readDouble(uint32_t address){

}

