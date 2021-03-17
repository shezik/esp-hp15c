#include <AceRoutine.h>
#include <math.h>
#include <BigNumber.h>
#include <BasicLinearAlgebra.h>
#include <string.h>

bool On = true;
// var Display; (Object)
bool DecimalSwap;
double Stack = {0, 0, 0, 0};
double StackI = {0, 0, 0, 0};
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
// var LcdDisplay; (Object)
uint8_t DisplayMode = 1; // 1=FIX 2=SCI 3=ENG
uint8_t DisplayDigits = 4;
unsigned short FullCircle = 360;
double TrigFactor = M_PI / 180;
bool Flags[] = {false, false, false, false, false, false, false, false, false, false};
bool User = false;
bool Prgm = false;
Opcode Program[60]; // var Program = [null];
unsigned short PC = 0;
bool Running = false;
// var RunTimer = null;
bool BlinkOn = false;
// var Blinker = null;
unsigned short ReturnStack[60]; // var ReturnStack = [];
uint8_t Result = 0;

const double MAX = 9.999999999e99;
const uint8_t MAX_MAG = 99;

void setup() {
    Serial.begin(115200);

    randomSeed(analogRead(A0));

    // the following will be moved into constructors in the future

    u8g2.begin(); // Start U8g2 beforehand to display error messages to users

    // I2C comm setup for further use of MCP23016 GPIO expander i.e. handleMCPKey()
    Wire.begin(/*SDA=*/MCPSDA, /*SCL=*/MCPSCL);
    Wire.setClock(200000); // !!Not sure about this value
    writeBlockData(IODIR0, B00001111); // Configure first 4 pins as input and other 12 pins as output
    writeBlockData(IODIR1, B00000000);
}

void loop() {

}