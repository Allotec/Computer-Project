#include <iostream>
#include "Assembler.h"

int main(){
    char array[][20] = {
        {"add $v0, $zero, $t0"},
        {"slti $t1, $t2, 100 "},
        {"j 0x400008       "},
        {"sw $t1, 100($t2)   "},
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