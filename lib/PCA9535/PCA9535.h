#pragma once

#include "Arduino.h"
#include "Wire.h"

#define PCAA9535_DEFAULT_ADDRESS 0x20

#define PCA9535_INPUT_REG_0 0x00
#define PCA9535_INPUT_REG_1 0x01
#define PCA9535_OUTPUT_REG_0 0x02
#define PCA9535_OUTPUT_REG_1 0x03
#define PCA9535_INVERT_INPUT_REG_0 0x04
#define PCA9535_INVERT_INPUT_REG_1 0x05
#define PCA9535_CONFIG_REG_0 0x06
#define PCA9535_CONFIG_REG_1 0x07

#define PCA9535_PORT_0 0x00
#define PCA9535_PORT_1 0x01
#define PCA9535_PORT_OUTPUT 0x00
#define PCA9535_PORT_INPUT 0xff

#define _01_BIT0_INPUT_ 0x01
#define _02_BIT1_INPUT_ 0x02
#define _04_BIT2_INPUT_ 0x04
#define _08_BIT3_INPUT_ 0x08
#define _10_BIT4_INPUT_ 0x10
#define _20_BIT5_INPUT_ 0x20
#define _40_BIT6_INPUT_ 0x40
#define _80_BIT7_INPUT_ 0x80

#define PCA9535_PIN_0 0X00
#define PCA9535_PIN_1 0x01
#define PCA9535_PIN_2 0x02
#define PCA9535_PIN_3 0x03
#define PCA9535_PIN_4 0x04
#define PCA9535_PIN_5 0x05
#define PCA9535_PIN_6 0x06
#define PCA9535_PIN_7 0x07
typedef union
{
    struct
    {
        uint8_t bit1 : 1;
        uint8_t bit2 : 1;
        uint8_t bit3 : 1;
        uint8_t bit4 : 1;
        uint8_t bit5 : 1;
        uint8_t bit6 : 1;
        uint8_t bit7 : 1;
        uint8_t bit8 : 1;
    };
} bit_typedef;
//On default Register (2,3) (4,5) (6,7): 0xff(output high), 0x00(non-invert input), 0xff(input mode)
class PCA9535
{
public:
    PCA9535();
#if defined(ESP8266) || defined(ESP32)
    bool begin(const uint8_t address, uint8_t dataPin, uint8_t clockPin);
#endif
    bool begin(const uint8_t address);
    bool begin(const uint8_t address, TwoWire *wire);

    bool isConnected();

    void pinMode(uint8_t port, uint8_t pin, uint8_t mode);
    void portMode(uint8_t port, uint8_t mode);
    uint8_t checkMode(uint8_t port);
    bool checkMode(uint8_t port, uint8_t pin);

    void digitalWrite(uint8_t port, uint8_t state);
    void digitalWrite(uint8_t port, uint8_t pin, bool state);

    uint8_t digitalRead(uint8_t port);
    bool digitalRead(uint8_t port, uint8_t pin);

    void digitalToggle(uint8_t port, uint8_t pin);
    void digitalToggle(uint8_t port);

    uint8_t digitalOutputRead(uint8_t port);
    bool digitalOutputRead(uint8_t port, uint8_t pin);

    void readRegister();

private:
    uint8_t shadowRegister[8];
    bit_typedef *input_reg_0 = (bit_typedef *)&shadowRegister[PCA9535_INPUT_REG_0];
    bit_typedef *input_reg_1 = (bit_typedef *)&shadowRegister[PCA9535_INPUT_REG_1];
    bit_typedef *output_reg_0 = (bit_typedef *)&shadowRegister[PCA9535_OUTPUT_REG_0];
    bit_typedef *output_reg_1 = (bit_typedef *)&shadowRegister[PCA9535_OUTPUT_REG_1];
    bit_typedef *mode_reg_0 = (bit_typedef *)&shadowRegister[PCA9535_CONFIG_REG_0];
    bit_typedef *mode_reg_1 = (bit_typedef *)&shadowRegister[PCA9535_CONFIG_REG_1];
    bool returnBit(bit_typedef *source, uint8_t bit);
    void setBit(bit_typedef *source, uint8_t n, uint8 state);

    void writeCmd(uint8_t reg, uint8_t cmd);
    void writeCmd(uint8_t reg, uint16_t cmd);
    void readByte(uint8_t reg, uint8_t *val);
    void readBytes(uint8_t reg, uint8_t *val, uint8_t n);

    void resetInvert();
    uint16_t readFull();
    TwoWire *_wire;
    uint8_t _addr;
};