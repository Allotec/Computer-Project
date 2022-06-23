#include <iostream>
#include "Assembler.h"

#include <fstream>
#include <string>

int main(){
    // std::string line;
    // std::ifstream myfile ("C:\\Users\\champ\\OneDrive\\Personal Projects\\Computer Project\\Test Program\\mipsNoPseudo.asm");

    // if (myfile.is_open())
    // {
    //     while ( getline (myfile,line) )
    //     {
    //     std::cout << line << '\n';
    //     }
    //     myfile.close();
    // }

    char array[][20] = {
        {"move $t0, $v0    "},
        {"slti $t1, $t2, 100 "},
        {"label:             "},
        {"j label            "},
        {"addiu $sp,$sp,-16  "},
        {"blt $t1, $t2, l2   "},
        {"l2:                "},
        {"li $t0, 0xFFFFF    "},
        {"                   "}
    };

    uint32_t* instruction = assemble(array, 20);

    for(int i = 0; *(instruction + i) != 0; i++){
        printf("%x\n", *(instruction + i));
    }

    delete[] instruction;
    return(0);
}