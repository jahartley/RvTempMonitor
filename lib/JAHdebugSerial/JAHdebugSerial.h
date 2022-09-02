
#ifdef DebugV
#define debugV(a) (DebugSerial.print(a))
#define debuglnV(a) (DebugSerial.println(a))
#else
#define debugV(a)
#define debuglnV(a)
#endif

#ifdef DebugI
#define debugI(a) (DebugSerial.print(a))
#define debuglnI(a) (DebugSerial.println(a))
#else
#define debugI(a)
#define debuglnI(a)
#endif
