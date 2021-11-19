






#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>//F. Malpartida LCD's driver
//#include <menu.h>//menu macros and objects
//#include <menuIO/lcdOut.h>//malpartidas lcd menu output
//#include <TimerOne.h>
//#include <ClickEncoder.h>
//#include <menuIO/clickEncoderIn.h>
//#include <menuIO/keyIn.h>
//#include <menuIO/chainStream.h>
//#include <menuIO/serialOut.h>
//#include <menuIO/serialIn.h>
//using namespace Menu;

LiquidCrystal_I2C lcd(0x3F,16,2);

// Encoder /////////////////////////////////////
#define encA 19
#define encB 20
//this encoder has a button here
#define encBtn 15



void setup() {
  // put your setup code here, to run once:
  SerialUSB.begin();
  lcd.init();
  lcd.backlight();

}

void loop() {
  // put your main code here, to run repeatedly:
  // when characters arrive over the serial port...
  if (SerialUSB.available()) {
    // wait a bit for the entire message to arrive
    delay(100);
    // clear the screen
    lcd.clear();
    lcd.backlight();
    // read all the available characters
    while (SerialUSB.available() > 0) {
      // display each character to the LCD
      lcd.write(SerialUSB.read());
    }
  }
}
