#include <AceRoutine.h>
#include <math.h>
#include <BigNumber.h>
#include <BasicLinearAlgebra.h>
#include <string.h>

bool On = true;
// var Display; (Object)
bool DecimalSwap;
double Stack = [0, 0, 0, 0];
double StackI = [0, 0, 0, 0];
double LastX = 0;
double LastXI = 0;
double Reg[60] = {0};
bool DisableKeys;
// var Entry; (String)
bool DigitEntry = false;
bool NewDigitEntry;
bool StackLift = false;
bool OldStackLift, NewStackLift;
uint8_t DelayUpdate = 0;
uint8_t DisplayTimeout = 0;
bool TemporaryDisplay = false;
uint8_t Shift = 0; // 1=f 2=g
Opcode (*Prefix)(uint8_t k); // var Prefix;
Opcode (*OldPrefix)(uint8_t k); // var OldPrefix;
// var LcdDisplay;
uint8_t DisplayMode = 1; // 1=FIX 2=SCI 3=ENG
uint8_t DisplayDigits = 4;
int FullCircle = 360;
double TrigFactor = M_PI / 180;
bool Flags[] = [false, false, false, false, false, false, false, false, false, false];
bool User = false;
bool Prgm = false;
 