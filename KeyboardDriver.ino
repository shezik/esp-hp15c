#include <Wire.h>
#include <math.h>

// I love uint8_t

const uint8_t GP0        = 0x00;  // DATA PORT REGISTER 0
const uint8_t GP1        = 0x01;  // DATA PORT REGISTER 1
const uint8_t OLAT0      = 0x02;  // OUTPUT LATCH REGISTER 0
const uint8_t OLAT1      = 0x03;  // OUTPUT LATCH REGISTER 1
const uint8_t IPOL0      = 0x04;  // INPUT POLARITY PORT REGISTER 0
const uint8_t IPOL1      = 0x05;  // INPUT POLARITY PORT REGISTER 1
const uint8_t IODIR0     = 0x06;  // I/O DIRECTION REGISTER 0
const uint8_t IODIR1     = 0x07;  // I/O DIRECTION REGISTER 1
const uint8_t INTCAP0    = 0x08;  // INTERRUPT CAPTURE REGISTER 0
const uint8_t INTCAP1    = 0x09;  // INTERRUPT CAPTURE REGISTER 1
const uint8_t IOCON0     = 0x0A;  // I/O EXPANDER CONTROL REGISTER 0
const uint8_t IOCON1     = 0x0B;  // I/O EXPANDER CONTROL REGISTER 1

const uint8_t MCPAddress = 0x20;  // Check out MCP23016 datasheet to find out how to determine this value
const uint8_t MCPSDA     = D2;
const uint8_t MCPSCL     = D1;
const uint8_t keyBindings[/*rows=*/4][/*cols=*/10] = { \
{1,  2,  3,  4,  5,  6,  7,  8,  9,  10}, \
{11, 12, 13, 14, 15, 16, 17, 18, 19, 20}, \
{21, 22, 23, 24, 25, 26, 27, 28, 29, 30}, \
{31, 32, 33, 34, 35, 36, 37, 38, 39, 40} }; // 10x4 keyboard key bindings

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
  Wire.endTransmission();
  //int ret = Wire.endTransmission();
  //Serial.print("Wire.endTransmission() from writeBlockData(uint8_t gp) finished with code ");
  //Serial.println(ret);
  delay(10);
}

uint8_t handleMCPKey() {

    // 14-pin 10x4 keyboard scanner, one keystroke at a time

    uint8_t coordRow, coordCol = 255;

    writeBlockData(GP0, 0b11110000);
    writeBlockData(GP1, 0b00111111);

    coordRow = (readBlockData(GP0) & 0b00001111);

    writeBlockData(GP0, 0b00000000);
    writeBlockData(GP1, 0b00000000);

    if (coordRow != 1 && coordRow != 2 && coordRow != 4 && coordRow != 8) {
        return 255;
    }

    coordRow = int(log(coordRow) / log(2)); // coordRow = 0, 1, 2, 3

    for (int i = 0; i < 4; i++) {
        writeBlockData(GP0, 0b00010000 << i);
        if (readBlockData(GP0) & 0b00001111) {
            coordCol = i; // coordCol = 0, 1, 2, 3
            break;
        }
    }

    writeBlockData(GP0, 0b00000000);

    if (coordCol != 255) {
        return keyBindings[coordRow][coordCol];
    }

    for (int i = 0; i < 10; i++) {
        writeBlockData(GP1, 0b00000001 << i);
        if (readBlockData(GP0) & 0b00001111) {
            coordCol = i; // coordCol = 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
            break;
        }
    }

    writeBlockData(GP1, 0b00000000);

    if (coordCol == 255) {
        return 255; // Key was released too early
    }

    return keyBindings[coordRow][coordCol + 4];

}