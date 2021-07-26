#pragma once

#include "Arduino.h"
#include "PCA9535.h"

#define LCD_RS_PIN 0     //0x01
#define LCD_RW_PIN 1     //0x02
#define LCD_EN_PIN 2     //0x04
#define LCD_LED_PIN 3    //0x08
#define LCD_DATA_PIN_4 4 //0x10
#define LCD_DATA_PIN_5 5 //0x20
#define LCD_DATA_PIN_6 6 //0x40
#define LCD_DATA_PIN_7 7 //0x80

#define LCD_FIRST_LINE 0x80
#define LCD_SECOND_LINE 0xC0
#define LCD_THIRD_LINE 0x90
#define LCD_FOURTH_LINE 0xD0
#define LCD_PORT PCA9535_PORT_0

#define LCD_CUSTOM_RIGHT 0
#define LCD_CUSTOM_LEFT 1
#define LCD_CUSTOM_UP_DOWN 2
#define LCD_CUSTOM_UP 3
#define LCD_CUSTOM_DOWN 4
// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set LCD_FUNCTIONSET
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00
// Change SDA, SCL pin in the lcd.c file
typedef enum
{
    PCA9535_8_BIT,
    PCA9535_4_BIT,
    DIRECT,
} lcd_mode_t;
class LCD
{
public:
    LCD();
    void begin(uint8_t _address, lcd_mode_t mode);
    void begin();
    void clear(void);
    void blink(bool state);
    void cursor(bool state);
    void led(bool state);
    void scrollDisplayLeft(bool state);
    void rightToLeft(bool state);
    void autoscroll();
    void noAutoscroll();

    void setRowOffsets(uint8_t row1, uint8_t row2, uint8_t row3, uint8_t row4);
    void createChar(uint8_t, uint8_t[]);

    void setCursor(uint8_t, uint8_t);

    uint8_t getcursor();
    uint8_t getrow();
    void movecursor(int step);
    void gotoX(uint8_t x);
    void gotoXY(uint8_t x, uint8_t y);
    void gotoend();
    void gotobegin();

    void write(uint8_t data);
    void writenotmove(uint8_t data);
    void printdata(const char *buf, uint8_t len);
    void printbdata(const char *buf, uint8_t len);
    void printCenter(const char *buf, uint8_t len);
    size_t printfdata(const char *format, ...);
    size_t printfCenter(const char *format, ...);
    size_t printfBackward(const char *format, ...);
    void nextline(bool rt);
    void previousline(bool rt);

    void clearRow();

private:
    uint8_t lcd_read_ac();
    uint8_t lcd_read_dr();
    void lcd_wait(bool ret_output);
    void lcd_cmd(uint8_t cmd);
    void lcd_data(uint8_t data);
    void init();
    void restartInit();
    void addChar(uint8_t addr, byte *character);
    PCA9535 pca;
    uint8_t _sda_pin, _scl_pin, _addr, _data, _ledState, _display, _row, _cursor, _function, _entry, _shift;
    uint8_t _rowOffset[4] = {0, 0, 0, 0};
};