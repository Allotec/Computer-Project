#include "Processor.h"
#include "Assembler.h"

Processor::Processor() : memUnit(3) {}

Processor::~Processor() {}

void Processor::singleStep(uint32_t instruction, uint32_t pc, bool &single){
    char* line = singleLineDis(instruction);
    char c;

    while(1){
        printLine(line, pc);
        c = getKeyboardChar();
        switch(c){
            case 'o':
                showOutput(intRegisters, floatRegisters, conditionalFlags, high, low);
                break;
            case 'm':
                showMemory(memUnit);
                break;
            case 's':
                delete [] line;
                return;
            case 'r':
                single = false;
                delete [] line;
                return;
            default:
                continue;
        }
    }

    return;
}

//Executes the Program
void Processor::executeProgram(uint32_t* program, bool single){
    this->program = program;

    //Reset all the values to zero inside the cpu
    for(int i = 0; i < 32; i++){
        intRegisters[i] = 0;
        floatRegisters[i] = 0;
    }
    
    for(int i = 0; i < 8; i++){
        conditionalFlags[i] = 0;
    }
    high = 0;
    low = 0;
    pc = 0;

    uint8_t opcode, fmt, ft, funct, opcodeIndex;
    char* instructionPointer;

    while(program[pc] != 0xFFFFFFFF){
        if(single)
            singleStep(program[pc], pc, single);

        opcode = ((program[pc] & bitM(6, 26)) >> 26);
        fmt = ((program[pc] & bitM(5, 21)) >> 21);
        ft = ((program[pc] & bitM(5, 16)) >> 16);
        funct = (program[pc] & bitM(6, 0));

        opcodeIndex = instructionLookup(opcode, funct, fmt, ft);

        if(program[pc] == 0){
            pc++;
            continue;
        }

        uint8_t rs = fmt, rt = ft, rd = ((program[pc] & bitM(5, 11)) >> 11), shamt = ((program[pc] & bitM(5, 6)) >> 6);
        int64_t temp;
        uint64_t tempUnsigned;
        int16_t imm = program[pc] & bitM(16, 0);
        uint32_t jumpAddress = program[pc] & bitM(26, 0);
        fDouble d1, d2, d3;

        switch(instructions[opcodeIndex].format){
            //R type
            case 0:
                //Instruction by funct code
                switch(funct){
                    //add and addu
                    case 0x20: case 0x21: 
                        intRegisters[rd] = intRegisters[rs] + intRegisters[rt]; 
                        break;
                    //jr
                    case 0x08: 
                        programCounter = intRegisters[rs]; 
                        break;
                    //nor
                    case 0x27: 
                        intRegisters[rd] = ~(intRegisters[rs] | intRegisters[rt]); 
                        break;
                    //or
                    case 0x25: 
                        intRegisters[rd] = intRegisters[rs] | intRegisters[rt]; 
                        break;
                    //slt
                    case 0x2a: 
                        intRegisters[rd] = intRegisters[rs] < intRegisters[rt] ? 1 : 0; 
                        break;
                    //sltu
                    case 0x2b:
                        intRegisters[rd] = (uint32_t)intRegisters[rs] < (uint32_t)intRegisters[rt] ? 1 : 0; 
                        break;
                    //sll
                    case 0x00: 
                        intRegisters[rd] = intRegisters[rt] << shamt; 
                        break;
                    //srl
                    case 0x02: 
                        intRegisters[rd] = ((uint32_t)intRegisters[rt]) >> shamt; 
                        break;
                    //sra
                    case 0x03: 
                        intRegisters[rd] = intRegisters[rt] >> shamt; 
                        break;
                    //sub and subu
                    case 0x22: case 0x23: 
                        intRegisters[rd] = intRegisters[rs] - intRegisters[rt]; 
                        break;
                    //div
                    case 0x1a: 
                        low = intRegisters[rs] / intRegisters[rt]; 
                        high = intRegisters[rs] % intRegisters[rt]; 
                        break;
                    //divu
                    case 0x1b: 
                        low = (uint32_t)intRegisters[rs] / (uint32_t)intRegisters[rt]; 
                        high = (uint32_t)intRegisters[rs] % (uint32_t)intRegisters[rt]; 
                        break;
                    //mfhi
                    case 0x10: 
                        intRegisters[rd] = high; 
                        break;
                    //mflo
                    case 0x12: 
                        intRegisters[rd] = low; 
                        break;
                    //mfc0 not implemented yet

                    //mult
                    case 0x18: 
                        temp = (int64_t)intRegisters[rs] * (int64_t)intRegisters[rt]; 
                        low = temp & 0x00000000FFFFFFFF; 
                        high = (temp & 0xFFFFFFFF00000000) >> 32; 
                        break;
                    //multu
                    case 0x19: 
                        tempUnsigned = (uint64_t)intRegisters[rs] * (uint64_t)intRegisters[rt]; 
                        low = tempUnsigned & 0x00000000FFFFFFFF; 
                        high = (tempUnsigned & 0xFFFFFFFF00000000) >> 32; 
                        break;
                    default:
                        break;
                }                
                
                break;

            //I type
            case 1:
                //Instruction by opcode
                switch(opcode){
                    //addi and addiu
                    case 0x08: case 0x09:
                        intRegisters[rt] = intRegisters[rs] + imm;

                        break;
                    //andi
                    case 0x0c:
                        intRegisters[rt] = intRegisters[rs] & ((uint32_t)imm & 0x0000FFFF);
                        break;
                    //beq
                    case 0x04:
                        if(intRegisters[rs] == intRegisters[rt]){
                            pc += 1 + imm;
                            continue;
                        }

                        break;
                    //bne
                    case 0x05:
                        if(intRegisters[rs] != intRegisters[rt]){
                            pc += 1 + imm;
                            continue;
                        }

                        break;
                    //ori
                    case 0x0d:
                        intRegisters[rt] = intRegisters[rs] | ((uint32_t)imm & 0x0000FFFF);
                        break;
                    //slti
                    case 0x0a:
                        intRegisters[rt] = intRegisters[rs] < imm ? 1 : 0;
                        break;
                    //sltiu
                    case 0x0b:
                        intRegisters[rt] = (uint32_t)intRegisters[rs] < ((uint32_t)imm & 0x0000FFFF) ? 1 : 0;
                        break;
                    
                    //lui
                    case 0x0F:
                        intRegisters[rt] &= 0x0000FFFF;
                        intRegisters[rt] |= ((uint32_t)imm) << 16;
                        break;
                    //Memory Instructions

                    //Load Instructions
                    //lbu
                    case 0x24:
                        intRegisters[rt] = (intRegisters[rt] & 0xFFFFFF00) | ((uint32_t)memUnit.readByte(intRegisters[rs] + imm) & 0x000000FF);
                        break;
                    //lhu
                    case 0x25:
                        intRegisters[rt] = (intRegisters[rt] & 0xFFFF0000) | ((uint32_t)memUnit.readHalfWord(intRegisters[rs] + imm) & 0x0000FFFF);
                        break;
                    //lw or ll 
                    case 0x23: case 0x30:
                        intRegisters[rt] = memUnit.readWord(intRegisters[rs] + imm);
                        break;
                    //lwc1
                    case 0x31:
                        floatRegisters[rt] = memUnit.readFloat(intRegisters[rs] + imm);
                        break;
                    //ldc1
                    case 0x35:
                        d1.num = memUnit.readDouble(intRegisters[rs] + imm);
                        floatRegisters[rt + 1] = d1.f1;
                        d1.bits = d1.bits >> 32;
                        floatRegisters[rt] = d1.f1;
                        break;
                    
                    //Store Instructions
                    //sb
                    case 0x28:
                        memUnit.writeAddress((uint32_t)(intRegisters[rs] + imm), (uint8_t)intRegisters[rt]);
                        break;
                    //sh
                    case 0x29:
                        memUnit.writeAddress((uint32_t)(intRegisters[rs] + imm), (uint16_t)intRegisters[rt]);
                        break;
                    //sw or sc
                    case 0x2b: case 0x38:
                        memUnit.writeAddress((uint32_t)(intRegisters[rs] + imm), (uint32_t)intRegisters[rt]);
                        break;
                    //swc1
                    case 0x39:
                        memUnit.writeAddress((uint32_t)(intRegisters[rs] + imm), floatRegisters[rt]);
                        break;
                    //sdc1
                    case 0x3d:
                        if(rt % 2 != 0)//Error doubles have to be on even numbered registers
                            break;

                        d1.f1 = floatRegisters[rt];
                        d1.bits = d1.bits << 32;
                        d1.f1 = floatRegisters[rt + 1];
                        memUnit.writeAddress((uint32_t)(intRegisters[rs] + imm), d1.num);
                        break;
                    
                    default:
                    break;
                }

                break;

            //J type
            case 2:
                switch(opcode){
                    //j
                    case 0x02:
                        pc = jumpAddress - 0x100000;
                        intRegisters[0] = 0; //Zero register should always be zero
                        continue;
                    //jal
                    case 0x03:
                        intRegisters[31] = pc + 8;
                        pc = jumpAddress - 0x100000;
                        intRegisters[0] = 0; //Zero register should always be zero
                        continue;
                }

                break;

            //FR type
            case 3:
                //For single precision numbers
                if(fmt == 0x10){
                    switch(funct){
                    //add.s
                    case 0x00:
                        floatRegisters[rd] = floatRegisters[rs] + floatRegisters[rt];
                        break;
                    //c.eq.s
                    case 0x32:
                        conditionalFlags[rd / 4] = floatRegisters[rs] == floatRegisters[rt] ? 0 : 1;
                        break;
                    //c.lt.s
                    case 0x3c:
                        conditionalFlags[rd / 4] = floatRegisters[rs] < floatRegisters[rt] ? 0 : 1;
                        break;
                    //c.le.s
                    case 0x3e:
                        conditionalFlags[rd / 4] = floatRegisters[rs] <= floatRegisters[rt] ? 0 : 1;
                        break;
                    //div.s
                    case 0x03:
                        floatRegisters[rd] = floatRegisters[rs] / floatRegisters[rt];
                        break;
                    //mul.s
                    case 0x02:
                        floatRegisters[rd] = floatRegisters[rs] * floatRegisters[rt];
                        break;
                    //sub.s
                    case 0x01:
                        floatRegisters[rd] = floatRegisters[rs] - floatRegisters[rt];
                        break;
                    default:
                    break;
                    }
                }
                //for double precision numbers
                else if(fmt == 0x11 && rd % 2 == 0 && rs % 2 == 0 && rt % 2 == 0){
                    //Combining rs and rs + 1 into a double by concatinating them
                    d2.bits = 0;
                    d2.f1 = floatRegisters[rs];
                    d2.bits = d2.bits << 32;
                    d2.f1 = floatRegisters[rs + 1];

                    //Combining rt and rt + 1 into a double by concatinating them
                    d3.bits = 0;
                    d3.f1 = floatRegisters[rt];
                    d3.bits = d2.bits << 32;
                    d3.f1 = floatRegisters[rt + 1];

                    switch(funct){
                    //add.d
                    case 0x00:
                        d1.num = d2.num + d3.num;
                        //Split the double into two 32 bit registers
                        floatRegisters[rd + 1] = d1.f1;
                        d1.bits = d1.bits >> 32;
                        floatRegisters[rd] = d1.f1;
                        break;
                    //c.eq.d
                    case 0x32:
                        conditionalFlags[rd / 4] = d2.num == d3.num ? 1 : 0;
                        break;
                    //c.lt.d
                    case 0x3c:
                        conditionalFlags[rd / 4] = d2.num < d3.num ? 1 : 0;
                        break;
                    //c.le.d
                    case 0x3e:
                        conditionalFlags[rd / 4] = d2.num <= d3.num ? 1 : 0;
                        break;
                    //div.d
                    case 0x03:
                        d1.num = d2.num / d3.num;
                        //Split the double into two 32 bit registers
                        floatRegisters[rd + 1] = d1.f1;
                        d1.bits = d1.bits >> 32;
                        floatRegisters[rd] = d1.f1;
                        break;
                    //mul.d
                    case 0x02:
                        d1.num = d2.num * d3.num;
                        //Split the double into two 32 bit registers
                        floatRegisters[rd + 1] = d1.f1;
                        d1.bits = d1.bits >> 32;
                        floatRegisters[rd] = d1.f1;
                        break;
                    //sub.d
                    case 0x01:
                        d1.num = d2.num - d3.num;
                        //Split the double into two 32 bit registers
                        floatRegisters[rd + 1] = d1.f1;
                        d1.bits = d1.bits >> 32;
                        floatRegisters[rd] = d1.f1;
                        break;
                    default:
                        break;
                    }
                }

                break;

            //FI type
            case 4:
                //bc1t
                if(ft % 4 == 1){
                    if(conditionalFlags[(ft - 1) % 4]){
                        pc += 1 + imm;
                        intRegisters[0] = 0; //Zero register should always be zero
                        continue;
                    }
                }
                //bc1f
                else if(ft % 4 == 0){
                    if(!conditionalFlags[ft % 4]){
                        pc += 1 + imm;
                        intRegisters[0] = 0; //Zero register should always be zero
                        continue;
                    }
                }
                break;

            //Error
            default:

                break;
        }

        intRegisters[0] = 0; //Zero register should always be zero
        pc++;
    }

    singleStep(program[pc], pc, single);
}
