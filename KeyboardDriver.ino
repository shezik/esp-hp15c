#include <Wire.h>

const int GP0        = 0x00;  // DATA PORT REGISTER 0
const int GP1        = 0x01;  // DATA PORT REGISTER 1
const int OLAT0      = 0x02;  // OUTPUT LATCH REGISTER 0
const int OLAT1      = 0x03;  // OUTPUT LATCH REGISTER 1
const int IPOL0      = 0x04;  // INPUT POLARITY PORT REGISTER 0
const int IPOL1      = 0x05;  // INPUT POLARITY PORT REGISTER 1
const int IODIR0     = 0x06;  // I/O DIRECTION REGISTER 0
const int IODIR1     = 0x07;  // I/O DIRECTION REGISTER 1
const int INTCAP0    = 0x08;  // INTERRUPT CAPTURE REGISTER 0
const int INTCAP1    = 0x09;  // INTERRUPT CAPTURE REGISTER 1
const int IOCON0     = 0x0A;  // I/O EXPANDER CONTROL REGISTER 0
const int IOCON1     = 0x0B;  // I/O EXPANDER CONTROL REGISTER 1
const int MCPAddress = 0x20;  // Check out MCP23016 datasheet to find out how to determine this value
const int MCPSDA     = D2;
const int MCPSCL     = D1;
const int keyBindings[/*rows=*/4][/*cols=*/4] = { \
{GEM_KEY_NONE, GEM_KEY_UP, GEM_KEY_NONE, GEM_KEY_OK}, \
{GEM_KEY_LEFT, GEM_KEY_DOWN, GEM_KEY_RIGHT, GEM_KEY_CANCEL}, \
{GEM_KEY_NONE, GEM_KEY_NONE, GEM_KEY_NONE, GEM_KEY_NONE}, \
{GEM_KEY_NONE, GEM_KEY_NONE, GEM_KEY_NONE, GEM_KEY_NONE} }; // 4x4 keyboard key bindings

uint8_t readBlockData(uint8_t gp)
{
  Wire.beginTransmission(MCPAddress);
  Wire.write(gp); 
  int ret = Wire.endTransmission();
  //Serial.print("Wire.endTransmission() from readBlockData(uint8_t gp) finished with code ");
  //Serial.println(ret);
  delay(10);
  Wire.requestFrom(MCPAddress, 1); //read one byte from chip
  delay(10);
  return Wire.read();
}

void writeBlockData(uint8_t reg, uint8_t data)
{
  Wire.beginTransmission(MCPAddress);
  Wire.write(reg);
  Wire.write(data);
  int ret = Wire.endTransmission();
  //Serial.print("Wire.endTransmission() from writeBlockData(uint8_t gp) finished with code ");
  //Serial.println(ret);
  delay(10);
}

int handleMCPKey() {
  // 8-pin 4x4 keyboard scanner, one keystroke at a time
  // running with MCP23016 GPIO expander
  // This scanning method consumes less CPU resource when idle

  //Serial.println();
  //Serial.println("handleMCPKey() was called!");
  
  uint8_t coordCol, coordRow;

  // Stage 1: Pull first 4 lanes high simultaneously to detect column
  writeBlockData(GP0, B00001111);
  coordCol = (readBlockData(GP0) & B11110000); // Only second 4 pins' status is kept. (one clue to possibly simplify this procedure: they'll always be greater or smaller in comparison to something, no idea if this could be useful or not)
  writeBlockData(GP0, B00000000); // Cleanup
  //Serial.print("Value of coordCol: ");
  //Serial.println(coordCol);

  if (coordCol != 16 && coordCol != 32 && coordCol != 64 && coordCol != 128) {
    //Serial.println("Multiple or no key was pressed!");
    return GEM_KEY_NONE; // Multiple or no key has been pressed, exiting
  }

  // Stage 2: Scan on second 4 lanes, i.e. pull them high individually, to detect row
  coordRow = -1; //Fallback value
  for (int i = 0; i < 4; i++) {
    writeBlockData(GP0, B00000001 << i); // Shift bits
    if (readBlockData(GP0) & B11110000) { // Scanning terminates immediately after a pressed key is found. Only one bit in coordRow can be 1. Think about it!
      coordRow = i;
      break;
    }
  }
  
  writeBlockData(GP0, B00000000); // Cleanup

  //Serial.print("Value of coordRow: ");
  //Serial.println(coordRow);

  if (coordRow == -1) { // If key is released too early
    return GEM_KEY_NONE;
  }

  return keyBindings[coordRow][int(log(coordCol) / log(2)) - 4]; // maths, yeah...
}