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
  delay(10); // Essential delays, comm does not work without them. :(
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

    // 14-pin 10x4 keyboard scanner, one keystroke at a time

    uint8_t coordRow, coordCol = -1;

    writeBlockData(GP0, B11110000);
    writeBlockData(GP1, B00111111);

    coordRow = (readBlockData(GP0) & B00001111);

    writeBlockData(GP0, B00000000);
    writeBlockData(GP1, B00000000);

    if (coordRow != 1 && coordRow != 2 && coordRow != 4 && coordRow != 8) {
        return -1;
    }

    coordRow = int(log(coordRow) / log(2)); // coordRow = 0, 1, 2, 3

    for (int i = 0; i < 4; i++) {
        writeBlockData(GP0, B00010000 << i);
        if (readBlockData(GP0) & B00001111) {
            coordCol = i; // coordCol = 0, 1, 2, 3
            break;
        }
    }

    writeBlockData(GP0, B00000000);

    if (coordCol != -1) {
        return keyBindings[coordRow][coordCol];
    }

    for (int i = 0; i < 10; i++) {
        writeBlockData(GP1, B00000001 << i);
        if (readBlockData(GP0) & B00001111) {
            coordCol = i; // coordCol = 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
            break;
        }
    }

    writeBlockData(GP1, B00000000);

    if (coordCol == -1) {
        return -1; // Key was released too early
    }

    return keyBindings[coordRow][coordCol + 4];

}