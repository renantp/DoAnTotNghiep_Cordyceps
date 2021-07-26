#pragma once

#include "Arduino.h"
#include "EEPROM.h"
#include "myEEPROM.h"
#include "LCD.h"
#include "RTClib.h"
#include <time.h>
#include "Arduino_JSON.h"
#include "time_keeper.h"
#define SELECT_S state_bt[0]
#define BACK_S state_bt[1]
#define UP_S state_bt[2]
#define DOWN_S state_bt[3]
#define MY_EEPROM_OUTPUT_ADDRESS (0x0004) //max 4095

#ifndef EEPROM_SETTING
#define EEPROM_SETTING (1024 - 65 - 65 - 512)
#endif
#define SCROLL_S _scroll_position != _scroll_prev

typedef struct
{
    float temp;
    float humi;
} sht_31_t;
typedef enum
{
    WELCOME,
    DASH_BOARD,
    WELCOME2,
    DETAIL,
    SETTING,
    ST_VIEW,
    ST_threshold,
    ST_OUTPUT,
    ST_DETAIL_TIME,
    ST_INPUT,
    ST_TEMP_FACTOR,
    ST_HUMI_FACTOR,
    ST_UPDATE,
    ST_TIME,
    ST_TIME_LIST,
    ST_WIFI,
    ST_INFO,
    CONTACT,
    YESNO,

    RELOAD_DUMY,
} page_t;
typedef enum
{
    SELECT,
    BACK,
    UP,
    DOWN,
} button_t;
//Setting struct
typedef struct
{
    bool blink = true;
    bool cursor = 0;
    bool led = 1;
} view_setting_t;
typedef struct
{
    uint8_t t_factor[8];
    uint8_t h_factor[8];
} st_input_t;
typedef struct
{
    float temp_h, temp_l, humi_h, humi_l;
} st_threshold_t;
typedef struct
{
    uint8_t state; //0: OFF to ON; 1: ON to OFF
    time_t date_time[2];
    uint8_t isUsed;
} st_timer_t;
typedef struct
{
    uint8_t type; //0-Temp, 1-Humi, 2-Timer, 3-Current/Temp, 4-Current/Humi, 5-Current/Timer, 6-Manual, 7-Current
    st_threshold_t threshold;
    uint8_t isHeater; // 1-Turn on when < threshold_L (Heater), 0-Turn on when > threshold_H
    float current;
    st_timer_t time_state[6];
} st_output_config_t;

typedef struct
{
    st_output_config_t relay_1, relay_2, out0, out1, out2;
} st_output_t;

typedef struct
{
    st_threshold_t *threshold;
    st_input_t *input;
    st_output_t *output;
    view_setting_t ui;

} ui_setting_t;
class LCD_UI
{
public:
    LCD_UI();
    void begin(uint8_t *button, sht_31_t *sht_val, ui_setting_t *setting_val);

    //PAGES

    void welcome_page();
    void welcome_page2();
    void dashboard_page(sht_31_t sht);
    void dashboard_refesh(sht_31_t sht);
    void detail_page(sht_31_t *sht);
    void detail_scroll();
    void detail_refesh(void);
    void setting_page();
    void setting_scroll();
    void st_view_page();
    void st_view_scroll();

    void st_input_page();
    void st_input_scroll();
    void st_input_temp_factor_page();
    void st_input_temp_factor_scroll();
    void st_input_humi_factor_page();
    void st_input_humi_factor_scroll();
    void st_output_page();
    void st_output_scroll();
    void st_time_main();
    void st_time_main_scroll();
    void st_time_main_logic();
    void st_threshold_page();
    void st_threshold_scroll();
    void st_threshold_logic();
    void st_threshold_on_change(uint8_t *scroll_p);
    void st_output_time_on_change(uint8_t *scroll_p);
    void st_time_page();
    void st_time_scroll();
    void st_time_logic();
    void st_info_page();
    void yesno_page();

    //Use??

    void optionPage(const char *title, void (*option)(), ...);
    void loop();
    ui_setting_t *setting;
    page_t _page, _page_ys;

    void saveFactorSetting(ui_setting_t *st);
    void readFactorSetting(ui_setting_t *st);
    void saveOutputSetting(ui_setting_t *st);
    void readOutputSetting(ui_setting_t *st);

    time_t rtc;
    struct tm rtc_tm;
    bool output_state[5];
    bool pre_output_state[5];
    sht_31_t sht_avg, pre_sht_avg;

    void updateRTC(time_t unixtime);
    void updateRTC(struct tm rtc_tm_struct);

private:
    void split(st_timer_t out);
    void st_output_time_detail_on_change_handle(uint8_t select_num);
    void st_output_time_detail_page(void);
    void st_output_time_detail_scroll(void);
    void st_output_time_detail_logic(void);
    void change_time_detail(time_t *val, bool isIncrease);
    uint8_t on_change_time = 0;
    uint8_t isHourChange = 0;
    st_timer_t *_chooseTimer;
    void option(const char *opt, time_t tm_val);
    String title;
    // Time

    //Option logic

    void detail_logic();
    void setting_logic();
    void st_input_logic();
    void st_input_temp_factor_logic();
    void st_input_humi_factor_logic();
    void st_factor_onchange(uint8_t *scroll_p);
    void st_output_logic();
    void st_output_onchange_handle(uint8_t scroll_num);
    void output_setting_detail_onchange_logic(uint8_t scroll_num, st_output_config_t *out);
    void output_setting_onchange_logic(uint8_t scroll_num);
    void output_setting_detail_page(st_output_config_t *out, const char *tilte);
    void st_view_logic();
    void st_info_logic();
    void yesno_logic();

    void optionOF(const char *buf, bool val, bool available);
    void option(const char *buf, float val, bool available);
    void option(const char *buf, uint8_t val, bool available);
    void option(const char *buf, bool available);
    JSONVar CustomJSON;
    void optionCustom(const char *buf, const char *ifTrue, const char *ifFalse, bool state);
    void optionCustom(const char *buf, JSONVar custom, uint8_t state);
    void optionYN(const char *yes, const char *no, bool priority /*=true*/);
    void question(const char *w);
    void split(uint8_t num, sht_31_t sht);
    void split(const char *col1, const char *col2, uint8_t st1, int st2);
    void split(const char *col1, const char *col2, const char *col3, uint8_t st1, uint8_t st2, uint8_t st3);
    void scroll(void *func, uint8_t step, bool back_top);

    //Navigator and Reactor Logic

    void navigator();
    void reactor();
    void page_handle(uint8 num, ...);
    void change_handle_float(float val);
    void change_handle_u8(uint8_t *val, uint8_t num);
    void change_value(bool incre, float *val, float h_limit, float l_limit, uint8_t dec);
    void change_value(bool incre, uint8_t *val, uint8_t h_limit, uint8_t l_limit);
    uint8_t on_change = 0;
    uint8_t on_change_threshold = 0;
    uint8_t on_change_current = 0;

    uint8_t change_p = 0;
    float pre_val;
    uint8_t pre_val_u8;
    void ptop();
    void clear_bt();
    void yesno();

    //View builder

    void scrollable(bool up, bool dw);
    void optional_view(uint8_t opt);
    void optional_view2(uint8_t opt);
    void optional_view1(void);
    void optionOF_change(bool val);

    void saveSetting(void);

    void selectThresholdOutput(st_output_config_t *this_output);

    uint8_t c_yn, c_yes, c_no, _scroll_position[10], _scroll_prev[10], _scroll_d, _scroll_u, _optional[4], _back;
    uint8_t *bt;
    uint8_t pre_bt[4];
    bool state_bt[4];
    uint8_t _state;
    void debounce();
    page_t _n_page;
    sht_31_t *sht;
    sht_31_t pre_sht = {.temp = 5.5, .humi = 5.5};
    float _current = 1.2;
    LCD lcd;
    ui_setting_t _setting;
    st_output_config_t *_chooseOutput;

protected:
};