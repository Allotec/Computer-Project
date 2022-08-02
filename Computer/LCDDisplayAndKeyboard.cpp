#include "LCDDisplayAndKeyboard.h"

extern LiquidCrystal_I2C lcd;
extern PS2Keyboard keyboard;
extern char** charArray;
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

void printArray(int row1, int row2, char** array){
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

void showOutput(int32_t* registers, float* fRegisters, bool* conditionalFlags, uint32_t high, uint32_t low){
  int row1 = 0, row2 = 3;
  char c;
  uint8_t page = 0;

  lcd.cursor_off();
  printOutput(registers, row1, row2);

  while(1){
    if(keyboard.available()){
      c = keyboard.read();
      if(c == '|'){
        return;
      }

      //Integer Registers
      if(page == 0){
        if(c == PS2_DOWNARROW && row2 < 31){
          printOutput(registers, ++row1, ++row2);
          c = '0';
        }
        else if(c == PS2_UPARROW && row1 > 0){
          printOutput(registers, --row1, --row2);
          c = '0';
        }
        //Transition to float
        else if(c == PS2_RIGHTARROW){
          page++;
          row1 = 0;
          row2 = 3;
          lcd.clear();
          printOutputFloat(fRegisters, row1, row2);
        }
      }
      //Float Registers
      else if(page == 1){
        if(c == PS2_DOWNARROW && row2 < 31){
          printOutputFloat(fRegisters, ++row1, ++row2);
          c = '0';
        }
        else if(c == PS2_UPARROW && row1 > 0){
          printOutputFloat(fRegisters, --row1, --row2);
          c = '0';
        }
        else if(c == PS2_LEFTARROW){
          page--;
          row1 = 0;
          row2 = 3;
          lcd.clear();
          printOutput(registers, row1, row2);
        }
        else if(c == PS2_RIGHTARROW){
          page++;
          row1 = 0;
          row2 = 3;
          lcd.clear();
          printOutputBool(conditionalFlags, row1, row2);
        }
      }
      //Conditional Flags
      else if(page == 2){
        if(c == PS2_DOWNARROW && row2 < 7){
          printOutputBool(conditionalFlags, ++row1, ++row2);
          c = '0';
        }
        else if(c == PS2_UPARROW && row1 > 0){
          printOutputBool(conditionalFlags, --row1, --row2);
          c = '0';
        }
        else if(c == PS2_LEFTARROW){
          page--;
          row1 = 0;
          row2 = 3;
          lcd.clear();
          printOutputFloat(fRegisters, row1, row2);
        }
        else if(c == PS2_RIGHTARROW){
          page++;
          row1 = 0;
          row2 = 3;
          lcd.clear();  
          lcd.setCursor(0, 0);
          lcd.print("High- 0x"); lcd.print(high, HEX);
          lcd.setCursor(0, 1);
          lcd.print("Low- 0x"); lcd.print(low, HEX);
        }
      }
      //Special registers
      else if(page == 3){
        if(c == PS2_LEFTARROW){
          page--;
          row1 = 0;
          row2 = 3;
          lcd.clear();
          printOutputBool(conditionalFlags, row1, row2);
        }
      }
    }
  }

}

void printOutput(int32_t* registers, int row1, int row2){
  int dispRow = 0;
  lcd.clear();

  for(int i = row1; i <= row2; i++){
    lcd.setCursor(0, dispRow++);
    lcd.print("R[");
    lcd.print(i);
    lcd.print("] = 0x");
    lcd.print(registers[i], HEX);
  }
}

void printOutputFloat(float* registers, int row1, int row2){
  int dispRow = 0;
  lcd.clear();

  for(int i = row1; i <= row2; i++){
    lcd.setCursor(0, dispRow++);
    lcd.print("F[");
    lcd.print(i);
    lcd.print("] = ");
    lcd.print(registers[i]);
  }
}

void printOutputBool(bool* flags, int row1, int row2){
  int dispRow = 0;
  lcd.clear();

  for(int i = row1; i <= row2; i++){
    lcd.setCursor(0, dispRow++);
    lcd.print("Flag[");
    lcd.print(i);
    lcd.print("] = ");
    lcd.print(flags[i]);
  }
}

char getKeyboardChar(){
  char c;

  while(1){
    if(keyboard.available()){
      c = keyboard.read();
      return(c);
    }
  }
}

void printLine(char* line, uint32_t pc){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line);
  lcd.setCursor(0, 1);
  lcd.print("Pc- ");
  lcd.print(pc);
  return;
}

void showMemory(MemoryController &memUnit){
  uint32_t row1 = 0, row2 = 3;
  char c;
  int32_t* regPtr = fetchMem(memUnit, row1, row2);
  char* line;
  printMemOutput(regPtr, row1, row2);

  while(1){
    if(keyboard.available()){
      c = keyboard.read();
      if(c == PS2_DOWNARROW && row2 < memUnit.getMaxAddress()){
          row1++;
          row2++;

          regPtr = fetchMem(memUnit, row1, row2);
          printMemOutput(regPtr, row1, row2);
          delete [] regPtr;
      }
      else if(c == PS2_UPARROW && row1 != 0){// && row1 > 0){
          row1--;
          row2--;

          regPtr = fetchMem(memUnit, row1, row2);
          printMemOutput(regPtr, row1, row2);
          delete [] regPtr;
      }
      //Search
      else if(c == 's'){
        line = keyboardBufferInput();
        row1 = arrayToNum(line);
        row2 = row1 + 3;
        
        //Clamping the output on both sides if its out of bounds
        if(row1 < 0){
            row1 = 0;
            row2 = 3;
        }
        else if(row2 > memUnit.getMaxAddress()){
          row2 = memUnit.getMaxAddress();
          row1 = row2 - 3;
        }
        
        regPtr = fetchMem(memUnit, row1, row2);
        printMemOutput(regPtr, row1, row2);
        
        delete [] line;
        delete [] regPtr;
      }
      else if(c == '|'){
        return;
      }

    }
  }    

}

void printMemOutput(int32_t* registers, uint32_t row1, uint32_t row2){
  uint32_t dispRow = 0, j = 0;
  lcd.clear();

  for(uint32_t i = row1; i <= row2; i++){
    lcd.setCursor(0, dispRow++);
    lcd.print("M[");
    lcd.print(i);
    lcd.print("] = 0x");
    lcd.print(registers[j], HEX);
    j++;
  }
}

int32_t* fetchMem(MemoryController &memUnit, uint32_t row1, uint32_t row2){
  int32_t* page = new int32_t[4];
  uint8_t j = 0;

  for(uint32_t i = row1; i <= row2; i++){
    page[j++] = memUnit.readByte(i);
  }

  return(page);
}

char* keyboardBufferInput(){
  char* line = new char[lineSize];
  char c;
  uint8_t position = 0;

  for(int i = 0; i < lineSize; i++){
    line[i] = 0;
  }
    
  lcd.clear();
  lcd.setCursor(position, 0);

  while(1){
    if(keyboard.available()){
      c = keyboard.read();

      if(c == PS2_RIGHTARROW && position < lineSize - 1){
        position++;
        lcd.setCursor(position, 0);
        continue;
      }
      else if(c == PS2_LEFTARROW && position > 0){
        position--;
        lcd.setCursor(position, 0);
        continue;
      }
      else if(c == PS2_ENTER){
        for(int i = position; i < lineSize; i++)
          line[i] = 0;
        
        return(line);
      }
      else if(c == PS2_BACKSPACE && position > 0){
        position--;
        line[position] = 0;
      }
      else{
        line[position] = c;
        position++;
      }

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(line);
      lcd.setCursor(position, 0);
    }
  }

}
