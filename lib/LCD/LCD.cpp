#include "LCD.h"
#include "Wire.h"
#include "string.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
LCD::LCD()
{
    _addr = PCAA9535_DEFAULT_ADDRESS;
    _sda_pin = SDA;
    _scl_pin = SCL;
    _ledState = 0x08;
    _display = LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSORON | LCD_BLINKON;
    _function = LCD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS;
    _row = LCD_FIRST_LINE;
    _cursor = _row;
    _entry = LCD_ENTRYMODESET | LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    _shift = LCD_CURSORSHIFT | LCD_CURSORMOVE | LCD_MOVERIGHT;
}
void LCD::begin(uint8_t _address, lcd_mode_t mode)
{
    _addr = _address;
#if defined(ESP8266) || defined(ESP32)
    pca.begin(_addr, _sda_pin, _scl_pin);
#else
    pca.begin(PCAA9535_DEFAULT_ADDRESS, Wire);
#endif
    if (!pca.isConnected())
        Serial.printf("[lcd_begin]Could not find the PCA9535 at address 0x%02x \r\n", _addr);
    init();
}
void LCD::begin()
{
#if defined(ESP8266) || defined(ESP32)
    pca.begin(_addr, _sda_pin, _scl_pin);
#else
    pca.begin(PCAA9535_DEFAULT_ADDRESS, Wire);
#endif
    delay(1000);
    init();
}

void LCD::write(uint8_t data)
{
    uint8_t entry = (_entry == (LCD_ENTRYMODESET | LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT)) ? 1 : 0;
    switch (data)
    {
    case '\r':
        if (entry)
            gotobegin();
        else
            gotoend();
        break;
    case '\n':
        if (entry)
            nextline(false);
        else
            previousline(false);
        break;
    case '\0':
        lcd_data(0);
        break;
    default:
        lcd_data(data);
        if (entry)
        {
            _cursor++;
            if ((_cursor - _row) >= 16)
            {
                nextline(true);
            }
        }
        else
        {
            if (_cursor == 0)
            {
                previousline(true);
                gotoend();
            }
            else
                _cursor--;
            // Serial.printf("[write]Row: %u, Cursor: %u\r\n", getrow(), getcursor());
        }

        break;
    }
}
void LCD::writenotmove(uint8_t data)
{
    lcd_data(data);
    lcd_cmd(_cursor);
}
void LCD::printdata(const char *buf, uint8_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        write(buf[i]);
    }
}
void LCD::printbdata(const char *buf, uint8_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        write(buf[len - 1 - i]);
    }
}
void LCD::printCenter(const char *buf, uint8_t len)
{
    uint8_t sta = (16 - len) / 2;
    _cursor = _row + sta;
    lcd_cmd(_cursor);
    printdata(buf, len);
}
size_t LCD::printfdata(const char *format, ...)
{
    va_list arg;
    va_start(arg, format);
    char temp[64];
    char *buffer = temp;
    size_t len = vsnprintf(temp, sizeof(temp), format, arg);
    va_end(arg);
    if (len > sizeof(temp) - 1)
    {
        buffer = new char[len + 1];
        if (!buffer)
        {
            return 0;
        }
        va_start(arg, format);
        vsnprintf(buffer, len + 1, format, arg);
        va_end(arg);
    }
    printdata(buffer, len);
    if (buffer != temp)
    {
        delete[] buffer;
    }
    return len;
}
size_t LCD::printfCenter(const char *format, ...)
{
    va_list arg;
    va_start(arg, format);
    char temp[64];
    char *buffer = temp;
    size_t len = vsnprintf(temp, sizeof(temp), format, arg);
    va_end(arg);
    if (len > sizeof(temp) - 1)
    {
        buffer = new char[len + 1];
        if (!buffer)
        {
            return 0;
        }
        va_start(arg, format);
        vsnprintf(buffer, len + 1, format, arg);
        va_end(arg);
    }
    printCenter(buffer, len);
    if (buffer != temp)
    {
        delete[] buffer;
    }
    return len;
}
size_t LCD::printfBackward(const char *format, ...)
{
    rightToLeft(true);
    va_list arg;
    va_start(arg, format);
    char temp[64];
    char *buffer = temp;
    size_t len = vsnprintf(temp, sizeof(temp), format, arg);
    va_end(arg);
    if (len > sizeof(temp) - 1)
    {
        buffer = new char[len + 1];
        if (!buffer)
        {
            return 0;
        }
        va_start(arg, format);
        vsnprintf(buffer, len + 1, format, arg);
        va_end(arg);
    }
    printbdata(buffer, len);
    if (buffer != temp)
    {
        delete[] buffer;
    }
    rightToLeft(false);
    return len;
}
void LCD::clear(void)
{
    lcd_cmd(LCD_CLEARDISPLAY);
    lcd_cmd(LCD_FIRST_LINE);
    _row = _cursor = LCD_FIRST_LINE;
}

void LCD::blink(bool state)
{
    if (state)
    {
        _display = _display | LCD_BLINKON;
        lcd_cmd(_display);
    }
    else
    {
        _display = _display & 0xfe;
        lcd_cmd(_display);
    }
}
void LCD::cursor(bool state)
{
    if (state)
    {
        _display = _display | LCD_CURSORON;
        lcd_cmd(_display);
    }
    else
    {
        _display = _display & 0xfd;
        lcd_cmd(_display);
    }
}
void LCD::setRowOffsets(uint8_t row1, uint8_t row2, uint8_t row3, uint8_t row4)
{
    _rowOffset[0] = row1;
    _rowOffset[1] = row2;
    _rowOffset[2] = row3;
    _rowOffset[3] = row4;
}
void LCD::led(bool state)
{
    if (state)
    {
        _ledState = 0x08;
        pca.digitalWrite(LCD_PORT, LCD_LED_PIN, 1);
    }
    else
    {
        _ledState = 0x00;
        pca.digitalWrite(LCD_PORT, LCD_LED_PIN, 0);
    }
}

void LCD::nextline(bool rt)
{
    uint8_t pre_cursor = getcursor();
    switch (_row)
    {
    case LCD_FIRST_LINE:
        _row = LCD_SECOND_LINE;
        break;
    case LCD_SECOND_LINE:
        _row = LCD_THIRD_LINE;
        break;
    case LCD_THIRD_LINE:
        _row = LCD_FOURTH_LINE;
        break;
    default:
        _row = LCD_FIRST_LINE;
        break;
    }
    if (rt)
    {
        _cursor = _row;
        lcd_cmd(_cursor);
    }
    else
    {
        _cursor = _row + pre_cursor;
        lcd_cmd(_cursor);
    }
}
void LCD::previousline(bool rt)
{
    uint8_t pre_cursor = getcursor();
    switch (_row)
    {
    case LCD_FIRST_LINE:
        _row = LCD_FOURTH_LINE;
        lcd_cmd(_row);
        break;
    case LCD_SECOND_LINE:
        _row = LCD_FIRST_LINE;
        lcd_cmd(_row);
        break;
    case LCD_THIRD_LINE:
        _row = LCD_SECOND_LINE;
        lcd_cmd(_row);
        break;
    default:
        _row = LCD_THIRD_LINE;
        lcd_cmd(_row);
        break;
    }
    if (rt)
        _cursor = _row;
    else
    {
        _cursor = _row + pre_cursor;
        lcd_cmd(_cursor);
    }
}
void LCD::movecursor(int step)
{
    int posible_step = (getcursor() + step);
    while (posible_step >= 16)
    {
        nextline(true);
        posible_step = 16 - posible_step;
    }
    while (posible_step < 0)
    {
        previousline(true);
        posible_step = 16 + posible_step;
    }
    _cursor = _row + posible_step;
    lcd_cmd(_cursor);
}
void LCD::gotoX(uint8_t x)
{
    x = x > 15 ? 15 : x;
    _cursor = _row + x;
    lcd_cmd(_cursor);
}
void LCD::gotoXY(uint8_t x, uint8_t y)
{
    x = x > 15 ? 15 : x;
    y = y > 3 ? 3 : y;
    switch (y)
    {
    case 1:
        _row = LCD_SECOND_LINE;
        lcd_cmd(_row + x);
        break;
    case 2:
        _row = LCD_THIRD_LINE;
        lcd_cmd(_row + x);
        break;
    case 3:
        _row = LCD_FOURTH_LINE;
        lcd_cmd(_row + x);
        break;
    default:
        _row = LCD_FIRST_LINE;
        lcd_cmd(_row + x);
        break;
    }
    _cursor = _row + x;
}
void LCD::gotoend()
{
    gotoXY(15, getrow());
}
void LCD::gotobegin()
{
    lcd_cmd(_row);
    _cursor = _row;
}

void LCD::rightToLeft(bool state)
{
    if (state)
        _entry = LCD_ENTRYMODESET | LCD_ENTRYRIGHT | LCD_ENTRYSHIFTDECREMENT;
    else
        _entry = LCD_ENTRYMODESET | LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    lcd_cmd(_entry);
}
void LCD::scrollDisplayLeft(bool state)
{
    if (state)
        _entry = _entry | LCD_ENTRYSHIFTDECREMENT;
    else
        _entry = _entry | LCD_ENTRYSHIFTINCREMENT;
    lcd_cmd(_entry);
}
void LCD::clearRow()
{
    lcd_cmd(_row);
    printdata("                ", 16);
    previousline(true);
}

uint8_t LCD::getcursor()
{
    return (_cursor - _row);
}
uint8_t LCD::getrow()
{
    switch (_row)
    {
    case LCD_FIRST_LINE:
        return 0;
        break;
    case LCD_SECOND_LINE:
        return 1;
        break;
    case LCD_THIRD_LINE:
        return 2;
        break;
    default:
        return 3;
        break;
    }
}
uint8_t LCD::lcd_read_ac()
{
    uint8_t ac;
    pca.portMode(LCD_PORT, PCA9535_PORT_INPUT & 0xf0);
    pca.digitalWrite(LCD_PORT, LCD_RS_PIN, 0);
    pca.digitalWrite(LCD_PORT, LCD_RW_PIN, 1);
    pca.digitalWrite(LCD_PORT, LCD_EN_PIN, 1);
    delay(10);
    ac = pca.digitalRead(LCD_PORT) & 0xf0;
    pca.digitalWrite(LCD_PORT, LCD_EN_PIN, 0);
    pca.digitalWrite(LCD_PORT, LCD_EN_PIN, 1);
    delay(10);
    ac = ac | (pca.digitalRead(LCD_PORT) >> 4);
    pca.digitalWrite(LCD_PORT, LCD_EN_PIN, 0);
    return ac;
}
uint8_t LCD::lcd_read_dr()
{
    uint8_t dr;
    pca.portMode(LCD_PORT, PCA9535_PORT_INPUT & 0xf0);
    pca.digitalWrite(LCD_PORT, LCD_RS_PIN, 0);
    pca.digitalWrite(LCD_PORT, LCD_RW_PIN, 1);
    pca.digitalWrite(LCD_PORT, LCD_EN_PIN, 1);
    delay(10);
    dr = pca.digitalRead(LCD_PORT) & 0xf0;
    pca.digitalWrite(LCD_PORT, LCD_EN_PIN, 0);
    pca.digitalWrite(LCD_PORT, LCD_EN_PIN, 1);
    delay(10);
    dr = dr | (pca.digitalRead(LCD_PORT) >> 4);
    pca.digitalWrite(LCD_PORT, LCD_EN_PIN, 0);
    return dr;
}
void LCD::lcd_wait(bool ret_output)
{
    pca.pinMode(LCD_PORT, LCD_DATA_PIN_7, PCA9535_PORT_INPUT);
    while (pca.digitalRead(LCD_PORT, LCD_DATA_PIN_7))
    {
        delay(10);
    }
    if (ret_output)
    {
        pca.pinMode(LCD_PORT, LCD_DATA_PIN_7, PCA9535_PORT_OUTPUT);
    }
}
void LCD::lcd_cmd(uint8_t cmd)
{
    pca.digitalWrite(LCD_PORT, 0x00 | _ledState);
    pca.portMode(LCD_PORT, PCA9535_PORT_OUTPUT);
    _data = (cmd & 0xf0) | _ledState;
    pca.digitalWrite(LCD_PORT, _data);
    pca.digitalWrite(LCD_PORT, LCD_EN_PIN, 1);
    _data = ((cmd << 4) & 0xff) | _ledState;
    delay(1);
    pca.digitalWrite(LCD_PORT, LCD_EN_PIN, 0);
    pca.digitalWrite(LCD_PORT, _data);
    pca.digitalWrite(LCD_PORT, 0x04 | _data);
    pca.digitalWrite(LCD_PORT, LCD_EN_PIN, 1);
    delay(2);
    pca.digitalWrite(LCD_PORT, LCD_EN_PIN, 0);
    pca.portMode(LCD_PORT, 0xf0);
}
void LCD::lcd_data(uint8_t data)
{
    pca.digitalWrite(LCD_PORT, 0x00 | _ledState);
    pca.portMode(LCD_PORT, PCA9535_PORT_OUTPUT);
    _data = (data & 0xf0) | _ledState;
    pca.digitalWrite(LCD_PORT, _data | 0x01);
    // pca.digitalWrite(LCD_PORT, LCD_RS_PIN, 1);
    pca.digitalWrite(LCD_PORT, LCD_EN_PIN, 1);
    os_delay_us(40);
    _data = ((data << 4) & 0xff) | _ledState;
    pca.digitalWrite(LCD_PORT, LCD_EN_PIN, 0);
    pca.digitalWrite(LCD_PORT, _data | 0x01);
    pca.digitalWrite(LCD_PORT, LCD_EN_PIN, 1);
    ets_delay_us(40);
    pca.digitalWrite(LCD_PORT, LCD_EN_PIN, 0);
    pca.portMode(LCD_PORT, 0xf0);
}
void LCD::init()
{
    //For minimum error uncomment this
    restartInit();
    uint8_t t[5] = {LCD_CLEARDISPLAY, _function, _display, _entry, LCD_CLEARDISPLAY};

    // uint8_t t[5] = {0x20, _function, _display, _entry, LCD_CLEARDISPLAY};
    for (uint8_t each : t)
    {
        lcd_cmd(each);
        // delay(10);
    }
    lcd_cmd(_row); // Move the cursor to beginning of first line
    // nextline(true);
    // char myBuf[] = "Hi There! This is long text";
    // printdata(myBuf, strlen(myBuf));
}
void LCD::restartInit()
{
    pca.digitalWrite(LCD_PORT, 0x00 | _ledState);
    pca.portMode(LCD_PORT, PCA9535_PORT_OUTPUT);
    pca.digitalWrite(LCD_PORT, 0x20);
    pca.digitalWrite(LCD_PORT, LCD_EN_PIN, 1);
    delay(5);
    pca.digitalWrite(LCD_PORT, LCD_EN_PIN, 0);
    pca.portMode(LCD_PORT, 0xf0);
    byte customChar[8][8] = {
        {B01000, B01100, B01110, B01111, B01110, B01100, B01000, B00000}, //> 0
        {B00010, B00110, B01110, B11110, B01110, B00110, B00010, B00000}, //< 1
        {B00100, B01110, B11111, B00100, B11111, B01110, B00100, B00000}, //up/dw 2
        {B00100, B01110, B11111, B00100, B00100, B00100, B00100, B00000}, //up 3
        {B00100, B00100, B00100, B00100, B11111, B01110, B00100, B00000}, //down 4
        {B11011, B10001, B11111, B11111, B11111, B10001, B11011, B00000}, //ON [.] 5
        {B11011, B10001, B10001, B10001, B10001, B10001, B11011, B00000}, //OFF[ ] 6
    };
    for (uint8_t i = 0; i < 8; i++)
    {
        addChar(i, customChar[i]);
    }
}
void LCD::addChar(uint8_t addr, byte *character)
{
    addr &= 0x07;
    lcd_cmd(LCD_SETCGRAMADDR | (addr << 3));
    for (int i = 0; i < 8; i++)
    {
        lcd_data(character[i]);
    }
}