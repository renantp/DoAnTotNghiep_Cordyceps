#include "PCA9535.h"
// #define _DEBUG
bool isLT7(uint8_t t)
{
    if (t <= 7)
    {
        return true;
    }
    return false;
}
PCA9535::PCA9535()
{
    _addr = PCAA9535_DEFAULT_ADDRESS;
    shadowRegister[2] = shadowRegister[3] = 0xff;
    shadowRegister[4] = shadowRegister[5] = 0x00;
    shadowRegister[6] = shadowRegister[7] = 0xff;
}
#if defined(ESP8266) || defined(ESP32)
bool PCA9535::begin(const uint8_t address, const uint8_t dataPin, const uint8_t clockPin)
{
    if ((address < 0x20) || (address > 0x27))
    {
        return false;
    }
    _addr = address;
    _wire = &Wire;
    if ((dataPin < 255) && (clockPin < 255))
    {
        _wire->begin(dataPin, clockPin);
    }
    else
    {
        _wire->begin();
    }
    resetInvert();
    return true;
}
#endif
bool PCA9535::begin(const uint8_t address)
{
    return begin(address, &Wire);
    resetInvert();
}
bool PCA9535::begin(const uint8_t address, TwoWire *wire)
{
    if ((address < 0x20) || (address > 0x27))
    {
        return false;
    }
    _addr = address;
    _wire = wire;
    _wire->begin();
    resetInvert();
    return true;
}

bool PCA9535::isConnected()
{
    _wire->beginTransmission(_addr);
    int rv = _wire->endTransmission(true);
    return (rv == 0);
}

void PCA9535::readRegister()
{
    readBytes(0x00, shadowRegister, 8);
    for (int i = 0; i <= 7; i++)
    {
        Serial.printf("Reg %u: 0x%02x \r\n", i, shadowRegister[i]);
    }
}

void PCA9535::pinMode(uint8_t port, uint8_t pin, uint8_t mode)
{
    pin = pin <= 7 ? pin : 7;
    if (port == 0)
    {
        if (mode == 0)
        {
            shadowRegister[PCA9535_CONFIG_REG_0] = !(0x01 << pin) & shadowRegister[PCA9535_CONFIG_REG_0];
            writeCmd(PCA9535_CONFIG_REG_0, shadowRegister[PCA9535_CONFIG_REG_0]);
        }
        else
        {
            shadowRegister[PCA9535_CONFIG_REG_0] = (0x01 << pin) & shadowRegister[PCA9535_CONFIG_REG_0];
            writeCmd(PCA9535_CONFIG_REG_0, shadowRegister[PCA9535_CONFIG_REG_0]);
        }
    }
    else
    {
        if (mode == 0)
        {
            shadowRegister[PCA9535_CONFIG_REG_1] = !(0x01 << (pin - 8)) & shadowRegister[PCA9535_CONFIG_REG_1];
            writeCmd(PCA9535_CONFIG_REG_1, (uint8_t)shadowRegister[PCA9535_CONFIG_REG_0]);
        }
        else
        {
            shadowRegister[PCA9535_CONFIG_REG_1] = (0x01 << (pin - 8)) & shadowRegister[PCA9535_CONFIG_REG_1];
            writeCmd(PCA9535_CONFIG_REG_1, shadowRegister[PCA9535_CONFIG_REG_1]);
        }
    }
}
void PCA9535::portMode(uint8_t port, uint8_t mode)
{
    if (port == 0)
    {
        shadowRegister[PCA9535_CONFIG_REG_0] = mode;
        writeCmd(PCA9535_CONFIG_REG_0, mode);
    }
    else
    {
        shadowRegister[PCA9535_CONFIG_REG_0] = mode;
        writeCmd(PCA9535_CONFIG_REG_1, mode);
    }
}
uint8_t PCA9535::checkMode(uint8_t port)
{
    if (port == 0)
    {
        readByte(PCA9535_CONFIG_REG_0, &shadowRegister[PCA9535_CONFIG_REG_0]);
#ifdef _DEBUG
        Serial.printf("[checkModePort]Mode register 0: 0x%02x ", shadowRegister[PCA9535_CONFIG_REG_0]);
#endif
        return shadowRegister[PCA9535_CONFIG_REG_0];
    }
    else
    {
        readByte(PCA9535_CONFIG_REG_1, &shadowRegister[PCA9535_CONFIG_REG_1]);
#ifdef _DEBUG
        Serial.printf("[checkMode]Mode register %u: 0x%02x ", port, shadowRegister[PCA9535_CONFIG_REG_1]);
#endif
        return shadowRegister[PCA9535_CONFIG_REG_1];
    }
}
bool PCA9535::checkMode(uint8_t port, uint8_t pin)
{
    pin = pin <= 7 ? pin : 7;
    if (port == 0)
    {
        checkMode(port);
#ifdef _DEBUG
        Serial.printf("[checkMode]Mode register 0: 0x%02x ", shadowRegister[PCA9535_CONFIG_REG_0]);
#endif
        return returnBit(mode_reg_0, pin);
    }
    else
    {
        checkMode(port);
#ifdef _DEBUG
        Serial.printf("[checkMode]Mode register %u: 0x%02x ", port, shadowRegister[PCA9535_CONFIG_REG_1]);
#endif
        return returnBit(mode_reg_1, pin);
    }
}

void PCA9535::digitalWrite(uint8_t port, uint8_t state)
{
    if (port == 0)
    {
        shadowRegister[PCA9535_OUTPUT_REG_0] = state;
        writeCmd(PCA9535_OUTPUT_REG_0, shadowRegister[PCA9535_OUTPUT_REG_0]);
    }
    else
    {
        shadowRegister[PCA9535_OUTPUT_REG_1] = state;
        writeCmd(PCA9535_OUTPUT_REG_1, shadowRegister[PCA9535_OUTPUT_REG_1]);
    }
}
void PCA9535::digitalWrite(uint8_t port, uint8_t pin, bool state)
{
    if (port == 0)
    {
        setBit(output_reg_0, pin, state);
        digitalWrite(port, shadowRegister[PCA9535_OUTPUT_REG_0]);
    }
    else
    {
        setBit(output_reg_1, pin, state);
        digitalWrite(port, shadowRegister[PCA9535_OUTPUT_REG_1]);
    }
}

void PCA9535::digitalToggle(uint8_t port, uint8_t pin)
{
    if (port == 0)
    {
        setBit(output_reg_0, pin, !returnBit(output_reg_0, pin));
        digitalWrite(port, shadowRegister[PCA9535_OUTPUT_REG_0]);
    }
    else
    {
        setBit(output_reg_1, pin, !returnBit(output_reg_1, pin));
        digitalWrite(port, shadowRegister[PCA9535_OUTPUT_REG_1]);
    }
}
void PCA9535::digitalToggle(uint8_t port)
{
    if (port == 0)
    {
        shadowRegister[PCA9535_OUTPUT_REG_0] = ~shadowRegister[PCA9535_OUTPUT_REG_0];
        writeCmd(PCA9535_OUTPUT_REG_0, shadowRegister[PCA9535_OUTPUT_REG_0]);
    }
    else
    {
        shadowRegister[PCA9535_OUTPUT_REG_1] = ~shadowRegister[PCA9535_OUTPUT_REG_1];
        writeCmd(PCA9535_OUTPUT_REG_1, shadowRegister[PCA9535_OUTPUT_REG_1]);
    }
}

uint8_t PCA9535::digitalRead(uint8_t port)
{
    readBytes(PCA9535_INPUT_REG_0, &shadowRegister[PCA9535_INPUT_REG_0], 2);
    // readBytes(PCA9535_INPUT_REG_0, &shadowRegister[PCA9535_INPUT_REG_0], 2);
    if (port == 0)
        return shadowRegister[PCA9535_INPUT_REG_0];
    else
        return shadowRegister[PCA9535_INPUT_REG_1];
}
bool PCA9535::digitalRead(uint8_t port, uint8_t pin)
{
    digitalRead(port);
    if (port == 0)
        return returnBit(input_reg_0, pin);
    else
        return returnBit(input_reg_1, pin);
}

uint8_t PCA9535::digitalOutputRead(uint8_t port)
{
    if (port == 0)
    {
        readBytes(PCA9535_OUTPUT_REG_0, &shadowRegister[PCA9535_OUTPUT_REG_0], 2);
#ifdef _DEBUG
        Serial.printf("[Output read]0x%02x 0x%02x\r\n", shadowRegister[PCA9535_OUTPUT_REG_0], shadowRegister[PCA9535_OUTPUT_REG_1]);
#endif
        return shadowRegister[PCA9535_OUTPUT_REG_0];
    }
    else
    {
        readBytes(PCA9535_OUTPUT_REG_0, &shadowRegister[PCA9535_OUTPUT_REG_0], 2);
#ifdef _DEBUG
        Serial.printf("0x%02x 0x%02x\r\n", shadowRegister[PCA9535_OUTPUT_REG_0], shadowRegister[PCA9535_OUTPUT_REG_1]);
#endif
        return shadowRegister[PCA9535_OUTPUT_REG_1];
    }
}
bool PCA9535::digitalOutputRead(uint8_t port, uint8_t pin)
{
    digitalOutputRead(port);
    if (port == 0)
        return returnBit(output_reg_0, pin);
    else
        return returnBit(output_reg_1, pin);
}

void PCA9535::writeCmd(uint8_t reg, uint16_t cmd)
{
    _wire->beginTransmission(_addr);
    _wire->write(reg);
    _wire->write(cmd);
    _wire->write(cmd >> 8);
    _wire->endTransmission(true);
}
void PCA9535::writeCmd(uint8_t reg, uint8_t cmd)
{
    _wire->beginTransmission(_addr);
    _wire->write(reg);
    _wire->write(cmd);
    _wire->endTransmission(true);
}
void PCA9535::readByte(uint8_t reg, uint8_t *val)
{
    uint8_t u = 1;
    _wire->beginTransmission(_addr);
    _wire->write(reg);
    _wire->endTransmission(false);
    _wire->requestFrom(_addr, u);
    *val = _wire->read();
}
void PCA9535::readBytes(uint8_t reg, uint8_t *val, uint8_t n)
{
    uint8_t u = 2;
    for (uint8_t i = 0; i < n; i += 2)
    {
        _wire->beginTransmission(_addr);
        _wire->write(reg + i);
        _wire->endTransmission(false);
        _wire->requestFrom(_addr, u);
        val[i] = _wire->read();
        val[i + 1] = _wire->read();
        _wire->endTransmission(true);
    }
}
bool PCA9535::returnBit(bit_typedef *source, uint8_t bit)
{
    switch (bit)
    {
    case 0:
        return source->bit1;
        break;
    case 1:
        return source->bit2;
        break;
    case 2:
        return source->bit3;
        break;
    case 3:
        return source->bit4;
        break;
    case 4:
        return source->bit5;
        break;
    case 5:
        return source->bit6;
        break;
    case 6:
        return source->bit7;
        break;
    case 7:
        return source->bit8;
        break;
    default:
        return 0;
        break;
    }
}
void PCA9535::setBit(bit_typedef *source, uint8_t n, uint8 state)
{
    switch (n)
    {
    case 0:
        source->bit1 = state;
        break;
    case 1:
        source->bit2 = state;
        break;
    case 2:
        source->bit3 = state;
        break;
    case 3:
        source->bit4 = state;
        break;
    case 4:
        source->bit5 = state;
        break;
    case 5:
        source->bit6 = state;
        break;
    case 6:
        source->bit7 = state;
        break;
    case 7:
        source->bit8 = state;
        break;
    default:
        break;
    }
}

void PCA9535::resetInvert()
{
    writeCmd(PCA9535_INVERT_INPUT_REG_0, (uint8_t)0x00);
    writeCmd(PCA9535_INVERT_INPUT_REG_0, (uint8_t)0x00);
}
