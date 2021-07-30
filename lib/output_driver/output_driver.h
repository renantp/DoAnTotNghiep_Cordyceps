#pragma once

#include "PCA9535.h"
#include "lcd_ui.h"
#include "time_keeper.h"
#include "fir_filter.h"

#ifndef K1_PIN
#define K1_PIN 4
#define K2_PIN 5
#endif
class UnixToTime
{
protected:
    struct tm _tm;
    time_t *_time;

public:
    UnixToTime(time_t &tm);
    int hour() const { return _tm.tm_hour; }
    int min() const { return _tm.tm_min; }
    int sec() const { return _tm.tm_sec; }
    int wday() const { return _tm.tm_wday; }
    int mday() const { return _tm.tm_mday; }
    int mon() const { return _tm.tm_mon; }
    int year() const { return _tm.tm_year; }
    int yday() const { return _tm.tm_yday; }
    int isdst() const { return _tm.tm_isdst; }
};

class DriverClass
{
protected:
    template <typename T>
    int GreaterThanOrEqual(T &val1, T &val2)
    {
        if (val1 >= val2)
            return 1;
        return 0;
    }
    template <typename T>
    int LessThanOrEqual(T &val1, T &val2)
    {
        if (val1 <= val2)
            return 1;
        return 0;
    }
    void updateTheDay(time_t *input_time)
    {
        struct tm t;
        localtime_r(input_time, &t);
        // Serial.printf("Before Time: %02i:%02i - %02i/%02i/%02d\r\n", t.tm_hour, t.tm_min, t.tm_mday, t.tm_mon + 1, t.tm_year + 1900);
        t.tm_mon = toTime(ui->rtc).mon();
        t.tm_mday = toTime(ui->rtc).mday();
        t.tm_year = toTime(ui->rtc).year();
        // Serial.printf("After Time: %02i:%02i - %02i/%02i/%02d\r\n", t.tm_hour, t.tm_min, t.tm_mday, t.tm_mon + 1, t.tm_year + 1900);
        *input_time = mktime(&t);
    }

private:
    /* data */
    void OutPutThresholdDrive(st_output_config_t &output_config, bool *output_state, bool *pre_output_state, uint8_t output_pin);
    void CheckCurrent(st_output_config_t &output_config);
    void UpdateDate(void);
    FIRFilter_t fir;
    PCA9535 *myExpan;
    LCD_UI *ui;
    uint16_t adc_value;
    float adc_voltage;
    float fir_voltage;

public:
    DriverClass();
    static UnixToTime toTime(time_t &unixtime);
    void loop(void);
    int init(PCA9535 *_PCA9535, LCD_UI *ui);
    void outputThreshold(void);
    void outputTimePeroid(void);

    //Variable
    bool button[4];
    float current;
    int current_total;
};
extern DriverClass OutputDriver;
