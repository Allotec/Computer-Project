#include "Memory.h"

//Memory base class
//Constructor
Memory::Memory(){
    this->MEMAddr = 0;
}

Memory::Memory(uint8_t MEMNum){
    this->MEMAddr = MEMNum;
}

//Destructor
Memory::~Memory(){}

//Setter
void Memory::setMemAddr(uint8_t MEMAddr){
    this->MEMAddr = MEMAddr;
}

//Configure Pins
void Memory::init(){
    //Configure Address Pins
    for(int i = 0; i < addressPinsNum; i++){
        pinMode(addressPins[i], OUTPUT);
    }

    //Configure chip enable pins
    for(int i = 0; i < chipEnableSize; i++){
        pinMode(chipEnable[i], OUTPUT);
    }

    //Output and write enable
    pinMode(outputEnable, OUTPUT);
    pinMode(writeEnable, OUTPUT);

}

//Reads a byte of data from a memory element
uint8_t Memory::readAddress(uint16_t address){
    uint8_t data = 0;

    //Configure I/O as inputs
    for(int i = 0; i < dataPinsNum; i++){
        pinMode(dataPins[i], INPUT);
    }

    //Dummy Chip
    for(int i = 0; i < chipEnableSize; i++){
        if(7 & ((uint8_t)1 << i)){
            digitalWrite(chipEnable[i], HIGH);
        }
        else{
            digitalWrite(chipEnable[i], LOW);
        }
    }

    //Put the output enable Low
    digitalWrite(outputEnable, HIGH);
    
    //Put the write enable high
    digitalWrite(writeEnable, HIGH);

    //Check for bad address
    if(address > size){
        Serial.print("Not a valid address" + address);
        return(-1);
    }
    //Put address on pins
    else{
        for(int i = 0; i < addressPinsNum; i++){
            //If the bit is not a zero then put the pin high
            if(address & ((uint16_t)1 << i)){
                digitalWrite(addressPins[i], HIGH);
            }
            //If it is a zero put the pin low
            else{
                digitalWrite(addressPins[i], LOW);
            }
        }
    }
    
    //Enable the Chip
    for(int i = 0; i < chipEnableSize; i++){
        if(this->MEMAddr & ((uint8_t)1 << i)){
            digitalWrite(chipEnable[i], HIGH);
        }
        else{
            digitalWrite(chipEnable[i], LOW);
        }
    }

    //Read the data on the lines
    for(int i = 0; i < dataPinsNum; i++){
        //Wait for data to come in
        delayMicroseconds(1);
        data |= ((uint8_t)digitalRead(dataPins[i])) << i;
    }

    //Put the output enable Low
    digitalWrite(outputEnable, HIGH);

    //Dummy Chip
    for(int i = 0; i < chipEnableSize; i++){
        if(7 & ((uint8_t)1 << i)){
            digitalWrite(chipEnable[i], HIGH);
        }
        else{
            digitalWrite(chipEnable[i], LOW);
        }
    }

    return(data);
}


/*
Steps to write
Configure I/O as outputs
Put the address on the lines
Put the ouput enable HIGH
Enable the chip
Put the write enable low
Put the output enable low
wait a little
Put the output enable High
*/

//Writes a byte of data to a memory element
void Memory::writeAddress(uint16_t address, uint8_t data){
    //Configure I/O as output
    for(int i = 0; i < dataPinsNum; i++){
        pinMode(dataPins[i], OUTPUT);
    }

    //Dummy Chip
    for(int i = 0; i < chipEnableSize; i++){
        if(7 & ((uint8_t)1 << i)){
            digitalWrite(chipEnable[i], HIGH);
        }
        else{
            digitalWrite(chipEnable[i], LOW);
        }
    }

    //Put the output enable HIGH
    digitalWrite(outputEnable, HIGH);

    //Put the write enable HIGH
    digitalWrite(writeEnable, HIGH);

    //Put Address on the Lines
    //Check for bad address
    if(address > size){
        Serial.print("Not a valid address" + address);
        return;
    }
    //Put address on pins
    else{
        for(int i = 0; i < addressPinsNum; i++){
            //If the bit is not a zero then put the pin high
            if(address & ((uint16_t)1 << i)){
                digitalWrite(addressPins[i], HIGH);
            }
            //If it is a zero put the pin low
            else{
                digitalWrite(addressPins[i], LOW);
            }
        }
    }

    //Enable the Chip
    for(int i = 0; i < chipEnableSize; i++){
        if(this->MEMAddr & ((uint8_t)1 << i)){
            digitalWrite(chipEnable[i], HIGH);
        }
        else{
            digitalWrite(chipEnable[i], LOW);
        }
    }

    //Put data on the lines
    for(int i = 0; i < dataPinsNum; i++){
        if(data & ((uint8_t)1 << i)){
            digitalWrite(dataPins[i], HIGH);
        }
        //If it is a zero put the pin low
        else{
            digitalWrite(dataPins[i], LOW);
        }
    }

    //Low pulse on the lines
    digitalWrite(writeEnable, LOW);

    delayMicroseconds(1);

    digitalWrite(writeEnable, HIGH);

    //Disable Chip
    for(int i = 0; i < chipEnableSize; i++){
        if(7 & ((uint8_t)1 << i)){
            digitalWrite(chipEnable[i], HIGH);
        }
        else{
            digitalWrite(chipEnable[i], LOW);
        }
    }

}

//Reads 32 bits from memory
uint32_t Memory::fetchData(uint16_t address){
    if(address > size){
        Serial.print("Not a valid address" + address);
        return(-1);
    }

    uint32_t data = 0;

    for(int i = 0; i < 4; i++){
        data = (data << 8) | (uint32_t)readAddress(address + i);
    }

    return(data);
}

//Writes 32 bits of data to memory
void Memory::writeData(uint16_t address, uint32_t data){
    for(int i = 0; i < 4; i++){
        writeAddress(address + 3 - i, (uint8_t)((address & (0xFF << (8 * i))) >> (8 * i)));
    }
}

void Memory::test(){
    uint8_t data;

    Serial.print("Memory- ");
    Serial.println(MEMAddr);

    for(int i = 1; i < 16; i++){
        writeAddress(2 * i, 3 * i);
        data = readAddress(2 * i);
        Serial.print("Wrote- ");
        Serial.print(3 * i);
        Serial.print(" Read- ");
        Serial.println(data);
    }
}
