#include "Assembler.h"

#include <fstream>
#include <string>
#include <iomanip>
#include <vector>

int main(int argc, char** argv){
    std::string line;
    std::ifstream myfile ("C:\\Users\\Matthew Champagne\\OneDrive\\Personal Projects\\Computer Project\\TestProgram\\test.asm");
    std::ofstream output("Output.txt");
    std::vector<std::string> arrayLines;

    if (myfile.is_open()){
        while(!myfile.eof()){
            std::getline(myfile, line);
            arrayLines.push_back(line);
        }

        myfile.close();
    }
    else{
        std::cout << "File could not open\n";
        return(0);
    }
    
    char** array = new char*[arrayLines.size()];
    int length = arrayLines.size();
    int j = 0, extraLines = 0;

    for(int i = 0; i < length; i++){
        if(arrayLines[i].length() == 0){
            continue;
        }
        else if(arrayLines[i].find("li") == std::string::npos){
            extraLines++;
        }
        else if(arrayLines[i].find("bgt") == std::string::npos){
            extraLines++;
        }
        else if(arrayLines[i].find("bge") == std::string::npos){
            extraLines++;
        }
        else if(arrayLines[i].find("ble") == std::string::npos){
            extraLines++;
        }
        else if(arrayLines[i].find("blt") == std::string::npos){
            extraLines++;
        }

        array[j] = new char[arrayLines[i].length() + 1];
        array[j] = strcpy(array[j], arrayLines[i].c_str());
        j++;
    }

    uint32_t* instruction = assemble(array, j, extraLines);

    for(int i = 0; *(instruction + i) != 0xFFFFFFFF; i++){
        output << "0x" << std::hex << std::setw(8) << std::setfill('0') << *(instruction + i) << "\n";
    }

    delete[] instruction;
    return(0);
}