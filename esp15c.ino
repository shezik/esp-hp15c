#include <Arduino.h>
#include <AceRoutine.h>
#include <math.h>
// #include <BigNumber.h>
#include <BasicLinearAlgebra.h>
// #include <string.h>
#include <U8g2lib.h>
#include "Keyboard_10x4_MCP23016.h"

bool On = true;
// var Display; (Object)
bool DecimalSwap;
double Stack[4] = {0, 0, 0, 0};
double StackI[4] = {0, 0, 0, 0};
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
// Opcode (*Prefix)(uint8_t k); // var Prefix;
// Opcode (*OldPrefix)(uint8_t k); // var OldPrefix;
// var LcdDisplay; (Object)
uint8_t DisplayMode = 1; // 1=FIX 2=SCI 3=ENG
uint8_t DisplayDigits = 4;
uint16_t FullCircle = 360;
double TrigFactor = M_PI / 180;
bool Flags[] = {false, false, false, false, false, false, false, false, false, false};
bool User = false;
bool Prgm = false;
// Opcode Program[60]; // var Program = [null];
uint16_t PC = 0;
bool Running = false;
// var RunTimer = null;
bool BlinkOn = false;
// var Blinker = null;
uint16_t ReturnStack[60]; // var ReturnStack = [];
uint8_t Result = 0;

const double MAX = 9.999999999e99;
const uint8_t MAX_MAG = 99;

U8G2_ST7565_JLX12864_F_4W_SW_SPI u8g2(U8G2_R2, /*clock=*/D5, /*data=*/D7, /*cs=*/D8, /*dc=*/D6, /*reset=*/U8X8_PIN_NONE);

Keyboard_10x4_MCP23016 keyboard(/*MCPAddress=*/0x20, /*MCPSDA=*/D2, /*MCPSCL=*/D1);

void setup() {
    Serial.begin(115200);
    randomSeed(analogRead(A0));

    u8g2.begin(); // Start U8g2 beforehand to display error messages to users
    keyboard.init();
}

void loop() {

}
