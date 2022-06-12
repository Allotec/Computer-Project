#include <iostream>
#include "Assembler.h"

int main(){
    char array[][20] = {
        {"add $v0, $zero, $t0"},
        {"slti $1, $2, 100   "},
        {"j 1000             "},
        {"sw $1, 100($2)     "},
        {"mult $s2, $t5      "},
        {"                   "}
    };

    uint8_t size = programLength(array);
    uint32_t* instruction = assemble(array, size);

    for(int i = 0; i < size; i++){
        printf("%x\n", *(instruction + i));
    }

    delete[] instruction;
    return(0);
}