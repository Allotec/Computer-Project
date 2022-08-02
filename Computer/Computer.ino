#include "LCDDisplayAndKeyboard.h"
#include "Processor.h"
#include "Assembler.h"

char** charArray;
int charLine[ROWSARRAY];

LiquidCrystal_I2C lcd(0x27, COLUMNS, ROWS);
PS2Keyboard keyboard;
Processor CPU;

void setup() {
  Serial.begin(115200);

  charArray = new char*[ROWSARRAY];

  for(int i = 0; i < ROWSARRAY; i++){
    charArray[i] = new char[COLUMNS];
    for(int j = 0; j < COLUMNS; j++){
      charArray[i][j] = ' ';
    }
  }

  delay(50);

  //LCD Setup
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.cursor_on();

  //Keyboard Setup
  keyboard.begin(DataPin, IRQpin);
}

void loop() {
  uint32_t* instructions;
  char c = menuOption();

  switch(c){

    case 'N':
    case 'n':
      editProgram(true);
      
      break;

    case 'E':
    case 'e':
      instructions = assemble(charArray, ROWSARRAY, 0);
      CPU.executeProgram(instructions, true);
      delete [] instructions;

      break;

    case 'Z':
    case 'z':
      editProgram(false);
      break;
      
    default:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Not implemented");
      delay(1000);
      break;
  }
}

