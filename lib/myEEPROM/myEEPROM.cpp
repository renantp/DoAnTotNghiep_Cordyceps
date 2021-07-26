#include "myEEPROM.h"

myEEPROMClass::myEEPROMClass(void) : _data(0)
{
    _addr = AT24C32M_ADDRESS;
    _size = 1024;
    _sector = 0;
}
bool myEEPROMClass::begin(void)
{
    Wire.begin();
    Wire.beginTransmission(_addr);
    if (Wire.endTransmission() == 0)
        return 1;
    return 0;
}
bool myEEPROMClass::begin(uint8_t address)
{
    _addr = address;
    Wire.begin();
    Wire.beginTransmission(_addr);
    if (Wire.endTransmission() == 0)
        return 1;
    return 0;
}

bool myEEPROMClass::isConnected(void)
{
    Wire.beginTransmission(_addr);
    if (Wire.endTransmission() == 0)
        return 1;
    return 0;
}
void myEEPROMClass::write(uint16_t address, uint8_t data)
{
    // Wire.beginTransmission(_addr);
    // Wire.write((uint8_t)(address >> 8));
    // Wire.write((uint8_t)address);
    // Wire.write(data);
    // if (Wire.endTransmission() == 0)
    //     Serial.println("MyEEPROM been write!");
    write(address, &data, 1);
}
uint8_t sizeOfPage = 1;
void myEEPROMClass::write(uint16_t address, uint8_t *buf, uint32_t length)
{
    uint32_t j, i;
    uint32_t page = length / sizeOfPage;  // 2
    uint8_t remain = length % sizeOfPage; // 16
    for (j = 0; j < page; j++)
    {
        while (!isConnected())
        {
        }
        // Serial.printf("\r\n Page: %u 0x%04x ----------------------------------\r\n", j, address);
        Wire.beginTransmission(_addr);
        Wire.write((uint8_t)(address >> 8));
        Wire.write((uint8_t)(address & 0xFF));
        for (i = 0; i < sizeOfPage; i++)
        {
            Wire.write(buf[i + (uint32_t)sizeOfPage * j]);
            // Serial.printf("0x%02x ", buf[i + sizeOfPage * j]);
        }
        Wire.endTransmission();
        address += (sizeOfPage);
    }
    if (remain != 0)
    {
        Wire.beginTransmission(_addr);
        Wire.write((uint8_t)(address >> 8));
        Wire.write((uint8_t)(address & 0xFF));
        // Serial.printf("\r\n Remain: %u 0x%04x----------------------------------\r\n", remain, address);
        for (i = 0; i < remain; i++)
        {
            Wire.write(buf[i + (uint32_t)sizeOfPage * page]);
            // Serial.printf("0x%02x ", buf[i + sizeOfPage * page]);
        }
        Wire.endTransmission();
    }
    // Serial.println("\r\n====================END WRITE========================\r\n");
}

uint8_t myEEPROMClass::read(uint16_t address)
{
    uint8_t rt;
    read(address, &rt, 1);
    return rt;
}
void myEEPROMClass::read(uint16_t address, uint8_t *buf, uint32_t length)
{
    uint32_t j, i;
    uint32_t page = length / sizeOfPage;  // 2
    uint8_t remain = length % sizeOfPage; // 16
    // Serial.printf("\r\n[read]So luong page %u\r\n", page);
    // Serial.printf("[read]So luong remain %u\r\n", remain);
    for (j = 0; j < page; j++)
    {
        // Serial.printf("\r\n Page: %u 0x%04x ----------------------------------\r\n", j, address);
        while (!isConnected())
        {
        }

        Wire.beginTransmission(_addr);
        Wire.write((uint8_t)(address >> 8));
        Wire.write((uint8_t)(address & 0xFF));
        Wire.endTransmission();
        Wire.requestFrom(_addr, sizeOfPage);
        for (i = 0; i < sizeOfPage; i++)
        {
            buf[i + (uint32_t)sizeOfPage * j] = Wire.read();
            // Serial.printf("0x%02x ", buf[i + sizeOfPage * j]);
            // Serial.printf("0x%02x ", Wire.read());
        }
        address += (sizeOfPage);
    }
    if (remain != 0)
    {
        Wire.beginTransmission(_addr);
        Wire.write((uint8_t)(address >> 8));
        Wire.write((uint8_t)(address & 0xFF));
        Wire.endTransmission();
        Wire.requestFrom(_addr, remain);
        // Serial.printf("\r\n Remain: %u 0x%04x----------------------------------\r\n", remain, address);
        for (i = 0; i < remain; i++)
        {
            buf[i + (uint32_t)sizeOfPage * page] = Wire.read();
            // Serial.printf("0x%02x ", buf[i + sizeOfPage * page]);
        }
    }
    // Serial.println("\r\n===========================END READ=============================\r\n");
}

bool myEEPROMClass::commit()
{
    if (!_dirty)
        return true;
    if (!_data)
        return false;

    if (ESP.flashEraseSector(_sector))
    {
        if (ESP.flashWrite(_sector * SPI_FLASH_SEC_SIZE, reinterpret_cast<uint32_t *>(_data), _size))
        {
            _dirty = false;
            return true;
        }
    }

    DEBUGV("EEPROMClass::commit failed\n");
    return false;
}

myEEPROMClass myEEPROM;