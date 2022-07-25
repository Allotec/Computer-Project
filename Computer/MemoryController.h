#include "Memory.h"

class MemoryController{
private:
    uint8_t sramNum;
    Memory* sram;

public:
    //Writing to memory
    void writeAddress(uint32_t address, uint8_t data);
    void writeAddress(uint32_t address, uint16_t data); 
    void writeAddress(uint32_t address, uint32_t data);
    void writeAddress(uint32_t address, float data);
    void writeAddress(uint32_t address, double data);

    //Reading from Memory
    uint8_t readByte(uint32_t address);
    uint16_t readHalfWord(uint32_t address);
    uint32_t readWord(uint32_t address);
    float readFloat(uint32_t address);
    double readDouble(uint32_t address);

    MemoryController();
    MemoryController(uint8_t sramNum);
    ~MemoryController();
};
