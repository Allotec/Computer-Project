const int DataPin = 8;
const int IRQpin =  3;

const int COLUMNS = 20;
const int ROWS = 4;
const int ROWSARRAY = 30;

void printArray(int row1, int row2, char array[][COLUMNS]);
bool menuOptionsCheck(char c);
char menuOption();
void editProgram(bool clearArray);
void showOutput(int registers[]);
void printOuput(int registers[], int row1, int row2);