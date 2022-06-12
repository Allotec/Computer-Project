#include "Assembler.h"

uint8_t programLength(char array[][20]){
    uint8_t empty = 0;
    
    for(int i = 0; true; i++){
        for(int j = 0; j < 20; j++){
            if(array[i][j] == ' '){
                empty++;
            }
        }
        
        if(empty == 19 || i == 50){
            return(i);
        }
        else{
            empty = 0;
        }
    }
}

//Array is always size 20
uint32_t* assemble(char arr[][20], uint8_t length){
    //Opcode -> 0
    //Rd -> 1
    //Rs -> 2
    //Rt or imm or shamt -> 3 
    char splitArray[4][10];
    int arrayIndex = 0, arrayRow = 0;
    int i = 0;
    char* array;
    uint32_t* instructions = new uint32_t[length];

    for(int k = 0; k < length; k++){
        array = arr[k];

        //Tokenize each expression
        while(arrayIndex < 20 && arrayRow < 4){
            //If we are at the start of a word
            if(*(array + arrayIndex) != ' ' && *(array + arrayIndex) != ',' && *(array + arrayIndex) != '('){
                for(i = 0; *(array + arrayIndex) != ' ' && *(array + arrayIndex) != ',' && *(array + arrayIndex) != ')' && *(array + arrayIndex) != '(' && arrayIndex < 20; i++){
                    splitArray[arrayRow][i] = *(array + arrayIndex);
                    arrayIndex++;
                }

                //Fill the rest of the opcode index with zeros
                if(i < 7){
                    for(int j = i; j < 10; j++){
                        splitArray[arrayRow][j] = 0;
                    }
                }

                arrayRow++;
            }

            arrayIndex++;
        }

        instructions[k] = mipsInstruction(splitArray[0], splitArray[1], splitArray[2], splitArray[3]);

        arrayRow = 0;
        arrayIndex = 0;
    }

    return(instructions);
}


uint32_t mipsInstruction(char opcode[10], char rd[10], char rs[10], char rt[10]){
    uint32_t instruction = 0;
    uint8_t opcodeIndex = opcodeLookup(opcode);

    //All Combinations
    uint32_t opcodeV = instructions[opcodeIndex].opcode;
    uint32_t functV = instructions[opcodeIndex].funct;
    uint32_t rsV = regLookup(rs);
    uint32_t rtV = regLookup(rt);
    uint32_t rdV = regLookup(rd);
    uint32_t shamt = 0;


    switch (instructions[opcodeIndex].format){
        //R type
        case 0:            
            //Special Cases
            // SLL and SRL
            if(functV == 0x00 || functV == 0x02){
                rtV = rsV;
                rsV = 0;
                shamt = arrayToNum(rt);
                shamt <<= 6;
            }
            // jr
            else if(functV == 0x08){
                rsV = rdV;
                rdV = 0;
                rtV = 0;
            }
            // mfhi and mflo
            else if(functV == 0x10 || functV == 0x12){
                rsV = 0;
                rtV = 0;
            }
            // mul/multu, div/divu, and mfco
            else if(functV == 0x18 || functV == 0x19 || functV == 0x1a || functV == 0x1b || opcodeV == 0x10){
                rtV = rsV;
                rsV = rdV;
                rdV = 0;
            }

            //General Case
            rdV <<= 11;
            rtV <<= 16;
            rsV <<= 21;

            instruction = rdV | rtV | rsV| functV | shamt | opcodeV;

            break;
        
        //I type
        case 1:
            

            printf("\nrd- %x\nrt- %x\nrs- %x\nfunct- %x\nshamt- %x\nop- %x\n", rdV, rtV, rsV, functV, shamt, opcodeV);

            break;

        //J type
        case 2:


            break;

        //FR type
        case 3:


            break;

        //FI type
        case 4:


            break;

        //Error
        default:
            break;
    }

    return(instruction);
}

//Returns the index into the table for the given opcode
uint32_t opcodeLookup(char opcode[10]){
    for(uint32_t i = 0; i < instructionNum; i++){
        if(strcmp(instructions[i].name, opcode) == 0)
            return(i);
    }

    return(255);
}

//Returns the value of the register given the name
uint32_t regLookup(char reg[10]){
    for(int i = 0; i < regNum; i++){
        if(strcmp(registers[i].name, reg) == 0)
            return(registers[i].val);
    }

    return(255);
}

//Turns a char array into an integer
int arrayToNum(char num[10]){
    int8_t sign = 1;
    int val = 0;
    uint8_t power = 0;
    
    //Determines if the number is negative and shifts everything down if it is
    if(num[0] == '-'){
        sign = -1;
        int i = 1;

        for(i = 1; num[i] - '0' >= 0 && num[i] - '0' <= 9; i++){
            num[i - 1] = num[i];
        }

        num[i - 1] = 0;
    }

    for(int i = 9; i >= 0; i--){
        if(num[i] == '\0')
            continue;

        val += (num[i] - '0') * mathPow(10, power++);
    }

    return(val * sign);
}

int mathPow(int x, uint8_t y){
    int pow = 1;

    for(uint8_t i = 0; i < y; i++){
        pow *= x;
    }

    return(pow);
}
