#include <LiquidCrystal_I2C.h>
#include <PS2Keyboard.h>
#include <Wire.h>
#include "LCDDisplayAndKeyboard.h"
//#include "Processor.h"
#include "Memory.h"

char charArray[ROWSARRAY][COLUMNS];
int charLine[ROWSARRAY];

LiquidCrystal_I2C lcd(0x27, COLUMNS, ROWS);
PS2Keyboard keyboard;
//Processor CPU;
Memory Mem(4);

void setup() {
  Serial.begin(9600);

  for(int i = 0; i < ROWSARRAY; i++){
    for(int j = 0; j < COLUMNS; j++){
      charArray[i][j] = ' ';
    }
  }

  Mem.init();
  for(int i = 0; i < 5; i++){
    Mem.setMemAddr(i);
    Mem.test();
  }

  delay(500);

  //LCD Setup
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.cursor_on();

  //Keyboard Setup
  keyboard.begin(DataPin, IRQpin);
}

void loop() {
  char c = menuOption();

  switch(c){
    case 'N':
    case 'n':
      editProgram(true);
      
      break;

    case 'E':
    case 'e':
      

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

