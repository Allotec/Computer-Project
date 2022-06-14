#include <iostream>
#include "Assembler.h"

int main(){
    char array[][20] = {
        {"add $v0, $zero, $t0"},
        {"slti $t1, $t2, 100 "},
        {"                   "},
        {"j 0x400008       "},
        {"                   "},
        {"sw $t1, 100($t2)   "},
        {"                   "},
        {"mult $s2, $t5      "},
        {"                   "}
    };

    uint32_t* instruction = assemble(array, 9);

    for(int i = 0; *(instruction + i) != 0; i++){
        printf("%x\n", *(instruction + i));
    }

    delete[] instruction;
    return(0);
}