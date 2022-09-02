JAH serial debug wrapper.

Instructions: Place these defines in your code at the top, and comment out to stop compiling debug info
//Debug Settings comment out to disable
#define WaitForUsbSerial        //Wait for Debug Serial connection on startup, comment to turn off, especially if not connected.
#define DebugV                  //verbose debuging info... lots of info
#define DebugI                  //important debug info...
//NEVER COMMENT FOLLOWING LINE
#define DebugSerial SerialUSB   //Select which serial port to use for debug messages
#include <JAHdebugSerial.h>

Place the following into your Setup(){
  #if defined(DebugV) || defined(DebugI)
    DebugSerial.begin(); //activate DebugSerial device
    #ifdef WaitForUsbSerial
      while(!DebugSerial); //blocks till debug serial is connected... turn off if not connected to PC.
    #endif
  #endif
}

USAGE: change Serial.print() to debugV() or debugI() or Serial.println() to debuglnV() debuglnI() depending on if you want it to be Verbose or Information level debug info. If you want no debug or only one level at production, just comment out the define for the level you dont want. If you comment out a define DebugV or I the complier will ignore all of those in your code.
