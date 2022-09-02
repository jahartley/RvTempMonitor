# RvTempMonitor
RV temperature monitor over rs485/modbus

pin connections on DevEBox STM32F4xx_M (STM32F407VET6)
RS485 on Serial3
  DI to TX(PD8)
  RO to RX(PD9)
  DE to PD12
  RE to PD11

LCD i2c1
  SCL PB8
  SCA PB9

Rotary Encoder
  encA PE2
  encB PE3
  encBtn PE4

LTE modem
  on Serial1
  TX PA10
  RX PA9
  5 PE5
  6 PE6



LIBRARY ERRORS  ArduinoModbus defines ON as 1, messing up other librarys


ARDUINO MODBUS /src/libmodbus/modbus.c line 867 change ON : OFF to 1 : 0

/src/libmodbus/modbus.h remove defines for ON and OFF