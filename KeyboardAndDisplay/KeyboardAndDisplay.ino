#include <LiquidCrystal_I2C.h>
#include <PS2Keyboard.h>
#include <Wire.h>

const int DataPin = 8;
const int IRQpin =  3;

const int COLUMNS = 20;
const int ROWS = 4;

const int ROWSARRAY = 50;

void printArray(int row1, int row2, char array[][COLUMNS]);
bool menuOptionsCheck(char c);
char menuOption();
void editNewProgram();
void excuteProgram();

void parseOperands(char array[]);
bool equals(char ParsedInstruction[3], char instruction);

const char menu[][20] = {
  "(N) Edit New Program", 
  "(E) Execute Program ", 
  "(L) Load Program    ", 
  "(R) Rough           ",
  "(W) Word            "
  };

const char menuOptions[] = {
  'N', 'E', 'L', 'R', 'W'
  };

int line = 0;
int dispLine = 0;
int charLine[ROWSARRAY];
int botLine = 0;
int topLine = ROWS;

char c;
char charArray[ROWSARRAY][COLUMNS];

int operands[4];

LiquidCrystal_I2C lcd(0x27, COLUMNS, ROWS);

PS2Keyboard keyboard;

void setup() {
  for(int i = 0; i < ROWSARRAY; i++){
    for(int j = 0; j < COLUMNS; j++){
      charArray[i][j] = ' ';
    }
  }

  delay(500);

  //LCD Setup
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.cursor_on();

  //Keyboard Setup
  keyboard.begin(DataPin, IRQpin);
  
  Serial.begin(9600);
}

void loop() {
  c = menuOption();

  botLine = 0;
  topLine = ROWS;

  switch(c){
    case 'N':
    case 'n':
      editNewProgram();
      
      break;

    case 'E':
    case 'e':
      executeProgram();

      break;
    default:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Not implemented");
      delay(1000);
      break;
  }

}

char menuOption(){
  char c;

  lcd.cursor_off();
  printArray(botLine, topLine, menu);

  while(1){
    if(keyboard.available()){
      c = keyboard.read();

      if(c == PS2_DOWNARROW){
        printArray(++botLine, ++topLine, menu);
        c = '0';
      }
      else if(c == PS2_UPARROW){
        printArray(--botLine, --topLine, menu);
        c = '0';
      }
      else if(menuOptionsCheck(c)){
        lcd.cursor_on();
        return(c);
      }
      else{
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Not a valid option");
        delay(1000);
        printArray(botLine, topLine, menu);
      }
    }
  }
}

void printArray(int row1, int row2, char array[][COLUMNS]){
  int row = 0;
  lcd.clear();

  for(int i = row1; i < row2; i++){
    for(int j = 0; j < COLUMNS; j++){
      lcd.setCursor(j, row);
      
      if(array[i][j] != ' ')
        lcd.print(array[i][j]);
    }
    row++;
  }
}

bool menuOptionsCheck(char c){
  for(int i = 0; i < 5; i++){
    if(menuOptions[i] == c || menuOptions[i] == (c - ('a' - 'A'))){
      return(true);
    }
  }

  return(false);
}

void editNewProgram(){
  lcd.clear();

  for(int i = 0; i < ROWSARRAY; i++){
    for(int j = 0; j < COLUMNS; j++){
      charArray[i][j] = ' ';
    }
  }

  while(1){
    if (keyboard.available()) {
      c = keyboard.read();
      
      if(c == '|'){
        break;
      }
      else if (c == PS2_LEFTARROW && charLine[line] > 0) {
        charLine[line]--;
      } 
      else if (c == PS2_RIGHTARROW && charLine[line] < COLUMNS) {
        charLine[line]++;
      } 
      else if (c == PS2_UPARROW && line > 0) {
        if(line == botLine && botLine != 0){
          botLine--;
          topLine--;
          line--;
        }
        else{
          dispLine--;
          line--;
        }
      } 
      else if ((c == PS2_DOWNARROW || c == PS2_ENTER) && line < ROWSARRAY - 1) {
        if(line == topLine - 1){
          botLine++;
          topLine++;
          line++;
        }
        else{
          dispLine++;
          line++;
        }
      } 
      else if (c == PS2_DELETE) {
        if(charLine[line] == COLUMNS - 1 && charArray[line][charLine[line]] != ' '){
          charArray[line][charLine[line]] = ' ';
        }
        else if(charLine[line] > 0){
          charLine[line]--;
          charArray[line][charLine[line]] = ' ';
        }
      } else {
        
        // otherwise, just print all normal characters
        if(charLine[line] < COLUMNS - 1){
          charArray[line][charLine[line]] = c;
          charLine[line]++;
        }
        else{
          charArray[line][charLine[line]] = c;
        }
      }
      
      printArray(botLine, topLine, charArray);
      lcd.setCursor(charLine[line], dispLine);
    }
  }
}

/*
Instruction Formats

Operations
0 add rd, r1, r2
1 sub rd, r1, r2
2 adi rd, r1, const
3 mul rd, r1, r2
4 div rd, r1, r2

Conditional
5 beq r1, r2, line
6 bne r1, r2, line
7 bgt r1, r2, line
8 blt r1, r2, line

Comparison
9 slt rd, r1, r2
10 sli rd, r1, r2

Special
12 stop
*/

void executeProgram(){
  int registers[20];
  int currentLine = 0;

  while(1){
    parseOperands(charArray[currentLine]);

    switch(operands[0]){
      

      //Uknown command
      default:
        break;
    }

  }
}

void parseOperands(char array[]){
  char instruction[3];
  char charOperands[3][2];

  for(int i = 0; i < 3; i++){
    instruction[i] = array[i];
  }

  for(int i = 0; i < 3; i++){
    for(int j = 0; j < 2; j++){
      charOperands[i][j] = array[(4 * (1 + i)) + j];
    }
  }

  if(equals(instruction, "add")){
    operands[0] = 0;
  }

}

bool equals(char ParsedInstruction[3], char instruction[3]){
  for(int i = 0; i < 3; i++){
    if(ParsedInstruction[i] != instruction[i]){
      return(false);
    }
  }

  return(true);
}
