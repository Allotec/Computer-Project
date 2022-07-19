#include <LiquidCrystal_I2C.h>
#include <PS2Keyboard.h>
#include <Wire.h>
#include "LCDDisplayAndKeyboard.h"

extern LiquidCrystal_I2C lcd;
extern PS2Keyboard keyboard;
extern char charArray[ROWSARRAY][COLUMNS];
extern int charLine[ROWSARRAY];

char menuOption(){
  const int menuNum = 5;
  const char menu[][20] = {
  "(N) Edit New Program", 
  "(E) Execute Program ",
  "(Z) Edit Existing   ", 
  "(S) Save            ",
  "(L) Load            "
  };

  int botLine = 0;
  int topLine = ROWS;
  int line = 0;
  char c;

  lcd.cursor_off();
  printArray(botLine, topLine, menu);

  while(1){
    if(keyboard.available()){
      c = keyboard.read();

      if(c == PS2_DOWNARROW){
        if(topLine == menuNum){
          continue;
        }
        else{
          printArray(++botLine, ++topLine, menu);
        }
      }
      else if(c == PS2_UPARROW){
        if(botLine == 0){
          continue;
        }
        else{
          printArray(--botLine, --topLine, menu);
        }
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

void printArray(int row1, int row2, const char array[][COLUMNS]){
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
  const char menuOptions[] = {
    'N', 'E', 'Z', 'S', 'L'
    };

  for(int i = 0; i < 5; i++){
    if(menuOptions[i] == c || menuOptions[i] == (c - ('a' - 'A'))){
      return(true);
    }
  }

  return(false);
}

void editProgram(bool clearArray){
  int line = 0;
  int dispLine = 0;
  int botLine = 0;
  int topLine = ROWS;

  lcd.clear();
  lcd.setCursor(0, 0);

  if(clearArray){
    for(int i = 0; i < ROWSARRAY; i++){
      for(int j = 0; j < COLUMNS; j++){
        charArray[i][j] = ' ';
      }
      charLine[i] = 0;
    }
  }
  else{
    printArray(botLine, topLine, charArray);
    lcd.setCursor(charLine[line], dispLine);
  }

  while(1){
    if (keyboard.available()) {
      char c = keyboard.read();
      
      if(c == '|'){
        break;
      }
      else if (c == PS2_LEFTARROW) {
        if(charLine[line] == 0){
          continue;
        }
        else{
          charLine[line]--;
        }
      } 
      else if (c == PS2_RIGHTARROW) {
        if(charLine[line] == COLUMNS - 1){
          continue;
        }
        else{
          charLine[line]++;
        }
      } 
      else if (c == PS2_UPARROW) {
        if(line == 0){
          continue;
        }
        else if(line == botLine && botLine != 0){
          botLine--;
          topLine--;
          line--;
        }
        else{
          dispLine--;
          line--;
        }
      } 
      else if (c == PS2_DOWNARROW || c == PS2_ENTER) {
        if(line == ROWSARRAY - 1){
          continue;
        }
        else if(line == topLine - 1){
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

void showOutput(int registers[20]){
  int row1 = 0, row2 = 3;
  char c;

  lcd.cursor_off();
  printOuput(registers, row1, row2);

  while(1){
    if(keyboard.available()){
      c = keyboard.read();

      if(c == PS2_DOWNARROW && row2 < 19){
        printOuput(registers, ++row1, ++row2);
        c = '0';
      }
      else if(c == PS2_UPARROW && row1 > 0){
        printOuput(registers, --row1, --row2);
        c = '0';
      }
      else if(c == '|'){
        return;
      }
    }
  }

}

void printOuput(int registers[], int row1, int row2){
  int dispRow = 0;
  lcd.clear();

  for(int i = row1; i <= row2; i++){
    lcd.setCursor(0, dispRow++);
    lcd.print("R[");
    lcd.print(i);
    lcd.print("] = ");
    lcd.print(registers[i]);
  }
}