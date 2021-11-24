






#include <Arduino.h>
#include <Wire.h>
//#include <LiquidCrystal_I2C.h>//F. Malpartida LCD's driver
#include <LiquidCrystal_PCF8574.h>
#include <menu.h>//menu macros and objects
#include <menuIO/PCF8574Out.h>//malpartidas lcd menu output
//#include <TimerOne.h>
#include <ClickEncoder.h>
#include <menuIO/clickEncoderIn.h>
#include <menuIO/keyIn.h>
#include <menuIO/chainStream.h>
#include <menuIO/serialOut.h>
#include <menuIO/serialIn.h>

#include <SparkFun_LTE_Shield_Arduino_Library.h>
#include <ArduinoRS485.h>
#include <ArduinoModbus.h>



using namespace Menu;


// These define's must be placed at the beginning before #include "TimerInterrupt_Generic.h"
#if !( defined(STM32F0) || defined(STM32F1) || defined(STM32F2) || defined(STM32F3)  ||defined(STM32F4) || defined(STM32F7) || \
       defined(STM32L0) || defined(STM32L1) || defined(STM32L4) || defined(STM32H7)  ||defined(STM32G0) || defined(STM32G4) || \
       defined(STM32WB) || defined(STM32MP1) )
  #error This code is designed to run on STM32F/L/H/G/WB/MP1 platform! Please check your Tools->Board setting.
#endif
#include <TimerInterrupt_Generic.h>

LiquidCrystal_PCF8574 lcd(0x3F);

// Encoder /////////////////////////////////////
#define encA PB3
#define encB PB4
//this encoder has a button here
#define encBtn PA15

// Create a LTE_Shield object to be used throughout the sketch:
#define POWER_PIN 5
#define RESET_PIN 6
LTE_Shield lte(POWER_PIN, RESET_PIN);


ClickEncoder clickEncoder(encA,encB,encBtn,4);
ClickEncoderStream encStream(clickEncoder,1);
MENU_INPUTS(in,&encStream);

// Init STM32 timer TIM1
STM32Timer ITimer0(TIM1);
void TimerHandler0(void)
{
  // Doing something here inside ISR
  clickEncoder.service();
  //SerialUSB.println("Starting  ITimer0 OK, millis() = " + String(millis()));
}

#define TIMER0_INTERVAL_MS        1      // 1s = 1000ms


result doAlert(eventMask e, prompt &item);

result showEvent(eventMask e,navNode& nav,prompt& item) {
  SerialUSB.print("event: ");
  SerialUSB.println(e);
  return proceed;
}

int test=55;

result action1(eventMask e,navNode& nav, prompt &item) {
  SerialUSB.print("action1 event: ");
  SerialUSB.print(e);
  SerialUSB.println(", proceed menu");
  SerialUSB.flush();
  return proceed;
}

result action2(eventMask e,navNode& nav, prompt &item) {
  SerialUSB.print("action2 event: ");
  SerialUSB.print(e);
  SerialUSB.println(", quiting menu.");
  SerialUSB.flush();
  return quit;
}

int ledCtrl=LOW;

result myLedOn() {
  ledCtrl=LOW;
  return proceed;
}
result myLedOff() {
  ledCtrl=HIGH;
  return proceed;
}

TOGGLE(ledCtrl,setLed,"Led: ",doNothing,noEvent,noStyle//,doExit,enterEvent,noStyle
  ,VALUE("On",HIGH,doNothing,noEvent)
  ,VALUE("Off",LOW,doNothing,noEvent)
);

int selTest=0;
SELECT(selTest,selMenu,"Select",doNothing,noEvent,noStyle
  ,VALUE("Zero",0,doNothing,noEvent)
  ,VALUE("One",1,doNothing,noEvent)
  ,VALUE("Two",2,doNothing,noEvent)
);

int chooseTest=-1;
CHOOSE(chooseTest,chooseMenu,"Choose",doNothing,noEvent,noStyle
  ,VALUE("First",1,doNothing,noEvent)
  ,VALUE("Second",2,doNothing,noEvent)
  ,VALUE("Third",3,doNothing,noEvent)
  ,VALUE("Last",-1,doNothing,noEvent)
);

//customizing a prompt look!
//by extending the prompt class
class altPrompt:public prompt {
public:
  altPrompt(constMEM promptShadow& p):prompt(p) {}
  Used printTo(navRoot &root,bool sel,menuOut& out, idx_t idx,idx_t len,idx_t) override {
    return out.printRaw(F("special prompt!"),len);;
  }
};

MENU(subMenu,"Sub-Menu",showEvent,anyEvent,noStyle
  ,OP("Sub1",showEvent,anyEvent)
  ,OP("Sub2",showEvent,anyEvent)
  ,OP("Sub3",showEvent,anyEvent)
  ,altOP(altPrompt,"",showEvent,anyEvent)
  ,EXIT("<Back")
);

/*extern menu mainMenu;
TOGGLE((mainMenu[1].enabled),togOp,"Op 2:",doNothing,noEvent,noStyle
  ,VALUE("Enabled",enabledStatus,doNothing,noEvent)
  ,VALUE("disabled",disabledStatus,doNothing,noEvent)
);*/

//char hexDigit[] = "0123456789ABCDEF";
//char hexNr[] = {"0","x",hexDigit,hexDigit};
//char buf1[]= "0x11";

MENU(mainMenu,"Main menu",doNothing,noEvent,wrapStyle
  ,OP("OpA",action1,anyEvent)
  ,OP("OpB",action2,enterEvent)
  //,SUBMENU(togOp)
  ,FIELD(test,"Test","%",0,100,10,1,doNothing,noEvent,wrapStyle)
  ,SUBMENU(subMenu)
  ,SUBMENU(setLed)
  ,OP("LED On",myLedOn,enterEvent)
  ,OP("LED Off",myLedOff,enterEvent)
  ,SUBMENU(selMenu)
  ,SUBMENU(chooseMenu)
  ,OP("Alert test",doAlert,enterEvent)
  //,EDIT("Hex",buf1,hexNr,doNothing,noEvent,noStyle)
  ,EXIT("<Back")
);

#define MAX_DEPTH 2

/*
const panel panels[] MEMMODE={{0,0,16,2}};SerialUSB.println("Starting  ITimer0 OK, millis() = " + String(millis()));
navNode* nodes[sizeof(panels)/sizeof(panel)];
panelsList pList(panels,nodes,1);
idx_t tops[MAX_DEPTH];
lcdOut outLCD(&lcd,tops,pList);//output device for LCD
menuOut* constMEM outputs[] MEMMODE={&outLCD};//list of output devices
outputsList out(outputs,1);//outputs list with 2 outputs
*/


MENU_OUTPUTS(out,MAX_DEPTH
  ,LCD_OUT(lcd,{0,0,16,2})
  ,NONE
);

NAVROOT(nav,mainMenu,MAX_DEPTH,in,out);//the navigation root object

result alert(menuOut& o,idleEvent e) {
  if (e==idling) {
    o.setCursor(0,0);
    o.print("alert test");
    o.setCursor(0,1);
    o.print("[select] to continue...");
  }
  return proceed;
}

result doAlert(eventMask e, prompt &item) {
  nav.idleOn(alert);
  return proceed;
}

result idle(menuOut& o,idleEvent e) {
  switch(e) {
    case idleStart:o.print("suspending menu!");break;
    case idling:o.print("suspended...");break;
    case idleEnd:o.print("resuming menu.");break;
  }
  return proceed;
}



void setup() {
  // put your setup code here, to run once:
  SerialUSB.begin(); //activate USB CDC driver
  //pinMode(encBtn,INPUT_PULLUP);
  pinMode(LED_BUILTIN,OUTPUT);
  while(!SerialUSB);
  SerialUSB.println("Arduino Menu Library");SerialUSB.flush();
  lcd.begin(16, 2);
  lcd.setBacklight(1);

  nav.idleTask=idle;//point a function to be used when menu is suspended
  mainMenu[1].enabled=disabledStatus;
  nav.showTitle=false;
  lcd.setCursor(0, 0);
  lcd.print("Menu 4.x LCD");
  lcd.setCursor(0, 1);
  lcd.print("r-site.net");
  //lcd.setBacklight(90);

  // Interval in microsecs
  if (ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, TimerHandler0))
    SerialUSB.println("Starting  ITimer0 OK, millis() = " + String(millis()));
  else
    SerialUSB.println("Can't set ITimer0. Select another freq. or timer");
  delay(2000);
  clickEncoder.setAccelerationEnabled(false);
  ModbusRTUClient.begin(9600);
  if ( lte.begin(Serial1) ) {
      SerialUSB.println("Initialied SARA-R4 module on a hardware serial port.");
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  nav.poll();
   digitalWrite(LED_BUILTIN, ledCtrl);
   delay(100);//simulate a delay as if other tasks are running
}
