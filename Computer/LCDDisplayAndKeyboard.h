#pragma once
#include <LiquidCrystal_I2C.h>
#include <PS2Keyboard.h>
#include <Wire.h>
#include "MemoryController.h"
#include "Assembler.h"

/*
LCD Pins
    VCC -> Yellow
    GND -> Brown
    SCL -> Purple
    SDA -> Blue

Keyboard Pins
    VCC -> Blue
    GND -> Black
    White -> Pin 3
    Red -> 8

*/

//Constants
const int DataPin = 8;
const int IRQpin =  3;

const int COLUMNS = 20;
const int ROWS = 4;
const int ROWSARRAY = 30;

//Functions
void printArray(int row1, int row2, const char array[][COLUMNS]);
void printArray(int row1, int row2, char** array);
bool menuOptionsCheck(char c);
char menuOption();
void editProgram(bool clearArray);
void showOutput(int32_t* registers, float* fRegisters, bool* conditionalFlags, uint32_t high, uint32_t low);
void printOutput(int32_t* registers, int row1, int row2);
void printOutputFloat(float* registers, int row1, int row2);
void printOutputBool(bool* flags, int row1, int row2);
char getKeyboardChar();
void printLine(char* line, uint32_t pc);
void showMemory(MemoryController &memUnit);
int32_t* fetchMem(MemoryController &memUnit, uint32_t row1, uint32_t row2);
void printMemOutput(int32_t* registers, uint32_t row1, uint32_t row2);
char* keyboardBufferInput();