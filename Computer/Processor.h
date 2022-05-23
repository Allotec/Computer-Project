
//Class to execute program on the EEPROM
class Processor{
private:
    int registers[32];
    unsigned int programCounter = 0;
    unsigned int instructionRegister = 0;
    

public:
    
    Processor();
    ~Processor();
};