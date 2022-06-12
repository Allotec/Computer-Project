#include <stdint.h>
#include <cstring>
#include <iostream>

struct instruction{
    const char* name;
    //R -> 0
    //I -> 1
    //j -> 2
    //FR -> 3
    //FI -> 4
    uint8_t format;
    uint8_t opcode;
    uint8_t funct;
    uint8_t FMT;
    uint8_t FT;
};

const uint8_t instructionNum = 60;

const instruction instructions[] = {
    //name for op fnct   fmt   ft
    {"add", 0, 0, 0x20, 0, 0}, {"addi", 1, 0, 0x08, 0, 0}, {"addiu", 1, 0, 0x09, 0, 0},
    {"addu", 1, 0, 0x21, 0, 0}, {"and", 0, 0, 0x24, 0, 0}, {"andi", 1, 0, 0x0C, 0, 0},
    {"beq", 1, 0, 0x04, 0, 0}, {"bne", 1, 0, 0x05, 0, 0}, {"j", 2, 0, 0x02, 0, 0},
    {"jal", 2, 0, 0x03, 0, 0}, {"jr", 0, 0, 0x08, 0, 0}, {"lbu", 1, 0, 0x24, 0, 0},
    {"lhu", 1, 0, 0x25, 0, 0}, {"ll", 1, 0, 0x30, 0, 0}, {"lui", 1, 0, 0x0F, 0, 0},
    {"lw", 1, 0, 0x23, 0, 0}, {"nor", 0, 0, 0x27, 0, 0}, {"or", 0, 0, 0x25, 0, 0},
    {"ori", 1, 0, 0x0D, 0, 0}, {"slt", 0, 0, 0x2a, 0, 0}, {"slti", 1, 0, 0x0A, 0, 0},
    {"sltiu", 1, 0, 0x0B, 0, 0}, {"sltu", 0, 0, 0x2B, 0, 0}, {"sll", 0, 0, 0x00, 0, 0},
    {"srl", 0, 0, 0x02, 0, 0}, {"sb", 1, 0, 0x28, 0, 0}, {"sc", 1, 0, 0x38, 0, 0},
    {"sh", 1, 0, 0x29, 0, 0}, {"sw", 1, 0, 0x2b, 0, 0}, {"sub", 0, 0, 0x22, 0, 0},
    {"subu", 0, 0, 0x23, 0, 0}, {"bclt", 4, 0x11, 0, 0x08, 0x01}, {"bclf", 4, 0x11, 0, 0x08, 0},
    {"div", 0, 0, 0x1A, 0, 0}, {"divu", 0, 0, 0x1B, 0, 0}, {"add.s", 3, 0x11, 0, 0x10, 0},
    {"add.d", 3, 0x11, 0, 0x11, 0}, {"c.eq.s", 3, 0x11, 0x32, 0x10, 0}, {"c.lt.s", 3, 0x11, 0x3C, 0x10, 0},
    {"c.le.s", 3, 0x11, 0x3E, 0x10, 0}, {"c.eq.d", 3, 0x11, 0x32, 0x11, 0}, {"c.lt.d", 3, 0x11, 0x3C, 0x11, 0},
    {"c.le.d", 3, 0x11, 0x3E, 0x11, 0}, {"div.s", 3, 0x11, 0x03, 0x10, 0}, {"div.d", 3, 0x11, 0x03, 0x11, 0},
    {"mul.s", 3, 0x11, 0x02, 0x10, 0}, {"mul.d", 3, 0x11, 0x02, 0x11, 0}, {"mul.d", 3, 0x11, 0x02, 0x11, 0},
    {"sub.s", 3, 0x11, 0x01, 0x10, 0}, {"sub.d", 3, 0x11, 0x01, 0x11, 0}, {"lwcl", 1, 0x31, 0, 0, 0},
    {"ldcl", 1, 0x35, 0, 0, 0}, {"mfhi", 0, 0, 0x10, 0, 0}, {"mflo", 0, 0, 0x12, 0, 0},
    {"mfc0", 0, 0x10, 0, 0, 0}, {"mult", 0, 0, 0x18, 0, 0}, {"multu", 0, 0, 0x19, 0, 0},
    {"sra", 0, 0, 0x03, 0, 0}, {"swcl", 1, 0x39, 0, 0, 0}, {"sdcl", 1, 0x3D, 0, 0, 0}
};

struct regVal{
    const char *name;
    uint8_t val;
};

const uint8_t regNum = 33;

const regVal registers[] = {
    {"$zero", 0}, {"$0", 0}, {"$at", 1}, {"$v0", 2}, {"$v1", 3},
    {"$a0", 4}, {"$a1", 5}, {"$a2", 6}, {"$a3", 7}, {"$t0", 8},
    {"$t1", 9}, {"$t2", 10}, {"$t3", 11}, {"$t4", 12}, {"$t5", 13},
    {"$t6", 14}, {"$t7", 15}, {"$s0", 16}, {"$s1", 17}, {"$s2", 18},
    {"$s3", 19}, {"$s4", 20}, {"$s5", 21}, {"$s6", 22}, {"$s7", 23},
    {"$t8", 24}, {"$t9", 25}, {"$k0", 26}, {"$k1", 27}, {"$gp", 28},
    {"$sp", 29}, {"$fp", 30},{"$ra", 31}
};


uint32_t* assemble(char array[][20], uint8_t length);
uint8_t programLength(char arr[][20]);
uint32_t mipsInstruction(char opcode[10], char rd[10], char rs[10], char rt[10]);
uint32_t opcodeLookup(char opcode[10]);
uint32_t regLookup(char reg[10]);
int arrayToNum(char num[10]);
int mathPow(int x, uint8_t y);