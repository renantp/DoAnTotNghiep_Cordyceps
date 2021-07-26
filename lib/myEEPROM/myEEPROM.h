#pragma once
#include "Arduino.h"
#include "Wire.h"

#define AT24C32M_ADDRESS 0x50

class myEEPROMClass
{
private:
    uint8_t _addr;

public:
    myEEPROMClass(void);
    bool begin(void);
    bool begin(uint8_t address);
    bool isConnected(void);
    uint8_t read(uint16_t address);
    void read(uint16_t address, uint8_t *buf, uint32_t length);
    void write(uint16_t address, uint8_t data);
    void write(uint16_t address, uint8_t *buf, uint32_t length);
    bool commit();
    // ~myEEPROM();
    template <typename T>
    void get(int const address, T &t)
    {
        read(address, (uint8_t *)&t, sizeof(T));
    }
    template <typename T>
    void put(int const address, T &t)
    {
        write(address, (uint8_t *)&t, sizeof(T));
    }
    size_t length() { return _size; }

protected:
    uint32_t _sector;
    uint8_t *_data;
    size_t _size;
    bool _dirty;
};

extern class myEEPROMClass myEEPROM;