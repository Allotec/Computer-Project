#include "Assembler.h"

//Gets rid of the lines with all spaces
void deleteWhiteSpace(char** array, uint8_t maxSize){
    uint8_t row = 0;
    uint8_t empty = 0;
    uint8_t i;

    while(row < maxSize){
        for(uint8_t j = 0; j < 20; j++){
            if(array[row][j] == ' '){
                empty++;
            }
        }
        
        //If the row is blank shift everything down and blank out the last thing so there isn't two copies
        if(empty == 19){
            for(i = row + 1; i < maxSize; i++){
                for(uint8_t j = 0; j < 20; j++){
                    array[i - 1][j] = array[i][j];
                }
            }

            for(uint8_t j = 0; j < 20; j++){
                    array[i][j] = ' ';
            }

        }
        else{
            empty = 0;
            row++;
        }
    }

}

//Finds the first empty line
uint8_t programLength(char** array){
    deleteWhiteSpace(array, 9);
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
//Have to add a pass through the array to find the labels and put them in a table with their values then look them up when needed
uint32_t* assemble(char arr[][20], uint8_t maxSize){
    //Copy orignal array to not mess up its contents
    char** arrayT = new char*[maxSize + 1];
    
    for(int j = 0; j < maxSize; j++){
        arrayT[j] = new char[20];
        for (int k = 0; k < 20; k++){
            arrayT[j][k] = arr[j][k];
        }
    }

    uint8_t length = programLength(arrayT);
    //Opcode -> 0
    //Rd -> 1
    //Rs -> 2
    //Rt or imm or shamt -> 3 
    char splitArray[4][12];
    int arrayIndex = 0, arrayRow = 0;
    int i = 0;
    char* array;
    uint32_t programCounter = 0x400000;
    uint32_t* instructions = new uint32_t[length + 1];

    //Look for labels and store them with there values
    for(int k = 0; k < length; k++){
        
    }


    //Split up all the instructions
    for(int k = 0; k < length; k++){
        array = arrayT[k];

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
                    for(int j = i; j < 12; j++){
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

    //Delete temp array
    for(int i = 0; i < maxSize + 1; i++) {
        delete [] arrayT[i];
    }
    delete [] arrayT;

    instructions[length] = 0;

    return(instructions);
}


uint32_t mipsInstruction(char opcode[12], char rd[12], char rs[12], char rt[12]){
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
                instruction = shamt;
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
                opcodeV << 26;
                instruction = opcodeV;
            }

            //General Case
            rdV <<= 11;
            rtV <<= 16;
            rsV <<= 21;

            instruction = rdV | rtV | rsV| functV;

            break;
        
        //I type
        case 1:
            //Register swaps and immdiate calculation
            //case opcode rt, rs, imm
            if(rtV == 0xFF){
                //For beq and bne
                if(opcodeV == 0x04 || opcodeV == 0x05){
                    rtV = rdV;
                    rdV = (arrayToNum(rt) & 0x00FFFFFF) >> 2;;
                }
                else{
                    rtV = rdV;
                    rdV = arrayToNum(rt) & 0x0000FFFF;
                }
            }
            //case opcode rt, imm(rs)
            else if(rsV == 0xFF){
                rsV = rtV;
                rtV = rdV;
                rdV = arrayToNum(rs) & 0x0000FFFF;
            }
            
            //Shifting
            rtV <<= 16;
            rsV <<= 21;
            opcodeV <<= 26;

            instruction = rdV | rtV | rsV | opcodeV;
            

            break;

        //J type
        case 2:
            //For j and jal
            rdV = (arrayToNum(rd) & 0x00FFFFFF) >> 2;
            opcodeV <<= 26;

            instruction = rdV | opcodeV;

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

//Finds the position of the first character given in the array, if its not then -1 is returned
uint8_t labelFind(char* array, char c){
    for(uint8_t i = 0; *(array + i) != 0; i++){
        if(*(array + i) == c){
            return(i);
        }
    }

    return(-1);
}

//Returns the index into the table for the given opcode
uint32_t opcodeLookup(char opcode[12]){
    for(uint32_t i = 0; i < instructionNum; i++){
        if(strcmp(instructions[i].name, opcode) == 0)
            return(i);
    }

    return(255);
}

//Returns the value of the register given the name
uint32_t regLookup(char reg[12]){
    for(int i = 0; i < regNum; i++){
        if(strcmp(registers[i].name, reg) == 0)
            return(registers[i].val);
    }

    return(255);
}

//Turns a char array into an integer
int arrayToNum(char num[12]){
    int8_t sign = 1;
    int base = 10;
    int val = 0;
    uint8_t power = 0;
    
    //Determines if the number is negative and shifts everything down if it is
    if(num[0] == '-'){
        sign = -1;
        int i = 1;

        for(i = 1; num[i] - '0' >= 0 && num[i] - '0' <= 11; i++){
            num[i - 1] = num[i];
        }

        num[i - 1] = 0;
    }
    else if(num[0] == '0' && num[1] == 'x'){
        int i = 1;
        base = 16;

        for(int j = 0; j < 2; j++){
            for(i = 1; num[i] - '0' >= 0 && num[i] - '0' <= 11; i++){
                num[i - 1] = num[i];
            }

            num[i - 1] = 0;
        }
    }

    for(int i = 11; i >= 0; i--){
        if(num[i] == '\0')
            continue;

        val += (num[i] - '0') * mathPow(base, power++);
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
