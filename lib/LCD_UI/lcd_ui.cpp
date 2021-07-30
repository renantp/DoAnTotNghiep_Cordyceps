#include "lcd_ui.h"
#include "fir_filter.h"
#include <vector>
using namespace std;
sht_31_t sht_test = {
    .temp = 29.34,
    .humi = 85.53,
};
sht_31_t sht_full_test[8] = {
    {.temp = 11.11, .humi = 11.11},
    {.temp = 22.22, .humi = 22.22},
    {.temp = 33.33, .humi = 33.33},
};
LCD_UI::LCD_UI()
{
    c_yes = c_no = 0;
    _page = _n_page = WELCOME;
}
void LCD_UI::updateRTC(time_t unixtime)
{
    rtc = unixtime;
    localtime_r(&rtc, &rtc_tm);
    // Serial.printf("[updateRTC]UI Time: %02i:%02i - %02i/%02i/%02d\r\n", rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_mday, rtc_tm.tm_mon + 1, rtc_tm.tm_year + 1900);
}
void LCD_UI::updateRTC(struct tm rtc_tm_struct)
{
    rtc_tm = rtc_tm_struct;
    rtc = mktime(&rtc_tm);
}

void LCD_UI::begin(bool *button, sht_31_t *sht_val, ui_setting_t *setting_val)
{
    bt = button;
    sht = sht_val;
    setting = setting_val;
    // saveFactorSetting(setting);
    // saveOutputSetting(setting);
    CustomJSON[0] = "Temp";
    CustomJSON[1] = "Humi";
    CustomJSON[2] = "Time";
    CustomJSON[3] = "Temp&I";
    CustomJSON[4] = "Humi&I";
    CustomJSON[5] = "Time&I";
    CustomJSON[6] = "Manual";
    CustomJSON[7] = "Current";
    readFactorSetting(setting);
    readOutputSetting(setting);
    _setting = *setting;
    lcd.begin();
    for (int i = 0; i < 4; i++)
    {
        pre_bt[i] = bt[i] = 1;
    }
    // for (uint8_t i = 0; i < 8; i++)
    // {
    //     Serial.printf("Temp Factor %u: %u \r\n", i, setting->input->t_factor[i]);
    //     Serial.printf("Humi Factor %u: %u \r\n", i, setting->input->h_factor[i]);
    // }

    welcome_page();
    lcd.blink(setting->ui.blink);
    lcd.cursor(setting->ui.cursor);

    // EEPROM.put(EEPROM_SETTING + 16, setting_val->output);
    // EEPROM.commit();
    // myEEPROM.put(0x02, *setting_val->output);
    // saveOutputSetting(setting);
    // dashboard((float)28.75, (float)60.54);
    _chooseOutput = &setting->output->relay_1;
    _chooseTimer = &_chooseOutput->time_state[0];
}
void LCD_UI::loop()
{

    debounce();

    navigator();

    if (tk.time_flag.t500ms)
    {
        float sum_t, sum_h;
        uint8_t factor_sum_t, factor_sum_h;
        factor_sum_t = factor_sum_h = 0;
        sum_t = sum_h = 0;
        for (uint8_t i = 0; i < 8; i++)
        {
            if ((sht[i].temp != 0) && (sht[i].humi != 0))
            {
                sum_t = sum_t + (float)((sht[i].temp * setting->input->t_factor[i]) / 10);
                sum_h = sum_h + (float)((sht[i].humi * setting->input->h_factor[i]) / 10);
                factor_sum_t += setting->input->t_factor[i];
                factor_sum_h += setting->input->h_factor[i];
            }
        }
        factor_sum_t = factor_sum_t != 0 ? factor_sum_t : 1;
        factor_sum_h = factor_sum_h != 0 ? factor_sum_h : 1;
        sht_avg.temp = (sum_t / factor_sum_t) * 10;
        sht_avg.humi = (sum_h / factor_sum_h) * 10;
        if (_page == DASH_BOARD)
        {
            // Serial.printf("Avr %.2f %.2f %u %u\r\n", sht_avg.temp, sht_avg.humi, factor_sum_t, factor_sum_h);
            if ((sht_avg.temp != pre_sht_avg.temp) || (sht_avg.humi != pre_sht_avg.humi))
            {
                dashboard_refesh(sht_avg);
                pre_sht_avg.temp = sht_avg.temp;
                pre_sht_avg.humi = sht_avg.humi;
            }
        }
        else if (_page == DETAIL)
        {
            detail_refesh();
        }
    }
}
void LCD_UI::navigator()
{
    //Go to that page
    if (_page != _n_page)
    {
        switch (_page)
        {
        case DASH_BOARD:
            dashboard_page(pre_sht);
            break;
        case WELCOME:
            welcome_page();
            break;
        case WELCOME2:
            welcome_page2();
            break;
        case DETAIL:
            Serial.println("Go to DETAIL");
            detail_page(sht);
            break;
        case SETTING:
            Serial.println("Go to Setting");
            setting_page();
            break;
        case ST_INPUT:
            Serial.println("Go to Input setting");
            st_input_page();
            break;
        case ST_OUTPUT:
            Serial.println("Go to Output setting");
            st_output_page();
            break;
        case ST_DETAIL_TIME:
            Serial.println("Go to Output Time setting");
            st_output_time_detail_page();
            break;
        case ST_TEMP_FACTOR:
            Serial.println("Go to Temp Factor");
            st_input_temp_factor_page();
            break;
        case ST_HUMI_FACTOR:
            Serial.println("Go to Humi Factor");
            st_input_humi_factor_page();
            break;
        case ST_threshold:
            Serial.println("Go to threshold");
            st_threshold_page();
            break;
        case ST_TIME:
            Serial.println("Go to Time  Main");
            st_time_main();
            break;
        case ST_TIME_LIST:
            Serial.println("Go to Time List");
            st_time_page();
            break;
        case ST_VIEW:
            Serial.println("Go to View");
            st_view_page();
            break;
        case ST_INFO:
            Serial.println("Go to InFo");
            st_info_page();
            break;
        case YESNO:
            yesno_page();
            break;
        default:
            break;
        }
    }
    //In this page do what when button on scroll
    reactor();
    //Clear Button state
    clear_bt();
}
void LCD_UI::reactor()
{
    switch (_page)
    {
    case WELCOME:
        if (SELECT_S)
        {
            SELECT_S = 0;
            _page = WELCOME2;
        }
        break;
    case WELCOME2:
        if (BACK_S)
        {
            BACK_S = 0;
            SELECT_S = 0;
            _page = WELCOME;
        }
        else if (SELECT_S)
        {
            BACK_S = 0;
            SELECT_S = 0;
            _page = DASH_BOARD;
        }
        break;
    case DASH_BOARD:
        if (BACK_S)
        {
            _page = WELCOME2;
        }
        else if (UP_S)
        {
            // yesno();
        }
        else if (DOWN_S)
        {
            // yesno();
        }
        else if (SELECT_S)
        {
            // _scroll_position[0] = 0;
            // if (c_yn == c_yes)
            //     _page = DETAIL;
            // else
            //     _page = SETTING;
            _page = DETAIL;
        }
        break;
    case DETAIL:
        detail_logic();
        break;

    case SETTING:
        setting_logic();
        break;
    case ST_INPUT:
        st_input_logic();
        break;
    case ST_OUTPUT:
        st_output_logic();
        break;
    case ST_TIME:
        st_time_main_logic();
        break;
    case ST_TIME_LIST:
        st_time_logic();
        break;
    case ST_DETAIL_TIME:
        st_output_time_detail_logic();
        break;
    case ST_VIEW:
        st_view_logic();
        break;
    case ST_HUMI_FACTOR:
        st_input_humi_factor_logic();
        break;
    case ST_TEMP_FACTOR:
        st_input_temp_factor_logic();
        break;
    case ST_threshold:
        st_threshold_logic();
        break;
    case ST_INFO:
        st_info_logic();
        break;
    case YESNO:
        yesno_logic();
        break;
    default:
        uint8_t *scroll_p = &_scroll_position[2];
        uint8_t *scroll_r = &_scroll_prev[2];
        if (BACK_S)
        {
        }
        else if (SELECT_S)
        {
            //change_handle();
        }
        else if (DOWN_S && !on_change)
        {
            *scroll_p = *scroll_p > 3 ? 0 : *scroll_p + 1; // = max case
        }
        else if (UP_S && !on_change)
        {
            *scroll_p = *scroll_p == 0 ? 0 : *scroll_p - 1;
        }
        if (*scroll_p != *scroll_r)
        {
        }
        if (on_change)
        {
            //change some value
        }
        break;
    }
}
void LCD_UI::debounce()
{
    for (size_t i = 0; i < 4; i++)
    {
        if ((bt[i] == 0) && (pre_bt[i] == 1))
        {
            state_bt[i] = 1;
        }
        pre_bt[i] = bt[i];
    }
}
void LCD_UI::yesno()
{
    if (c_yn == c_yes)
    {
        lcd.write('>');
        c_yn = c_no;
        lcd.gotoX(c_yn);
        lcd.writenotmove(0);
    }
    else
    {
        lcd.write('>');
        c_yn = c_yes;
        lcd.gotoX(c_yn);
        lcd.writenotmove(0);
    }
}

//Pages and reactive pages

void LCD_UI::welcome_page()
{
    _n_page = _page;
    lcd.clear();
    lcd.printfCenter("WELCOME");
    lcd.nextline(true);
    lcd.printfdata("Product of Renan Electronic");
    lcd.nextline(false);
    lcd.gotoend();
    lcd.printfBackward("Countinue");
    lcd.writenotmove(0);
}
void LCD_UI::welcome_page2()
{
    _n_page = _page;
    lcd.clear();
    lcd.printfCenter("WELCOME 2");
    lcd.nextline(true);
    lcd.printfdata("Product of Renan Electronic");
    lcd.nextline(false);
    lcd.gotoend();
    lcd.printfBackward("Next");
    lcd.writenotmove(0);
}

void LCD_UI::dashboard_page(sht_31_t sht)
{
    _n_page = _page;
    lcd.clear();
    // lcd.write(1);
    lcd.printCenter("DASH BOARD", strlen("DASH BOARD"));
    lcd.gotoend();
    lcd.writenotmove(0);
    lcd.nextline(true);
    option("Temp", sht.temp, false);
    option("Humi", sht.humi, false);
    option("Current", _current, false);
    lcd.gotoend();
    // optionYN("Detail", "Setting", true);
}

void LCD_UI::detail_page(sht_31_t *sht)
{
    _n_page = _page;
    lcd.clear();
    lcd.write(1);
    lcd.printfCenter("DETAIL");
    lcd.gotoend();
    lcd.writenotmove(0);
    lcd.nextline(true);
    detail_scroll();
}
void LCD_UI::detail_scroll()
{
    uint8_t *scroll_p = &_scroll_position[0];
    uint8_t *scroll_r = &_scroll_prev[0];
    *scroll_p = *scroll_p >= 7 ? 0 : *scroll_p;
    *scroll_r = *scroll_p;
    lcd.gotoXY(0, 1);
    switch (*scroll_p)
    {
    case 0:
        split("Temp", "Humi", 1, 1);
        split(*scroll_p + 1, sht[*scroll_p]);
        split(*scroll_p + 2, sht[*scroll_p + 1]);
        scrollable(false, true);
        // scrollable(true, true);
        break;
    case 1:
        split(*scroll_p, sht[*scroll_p - 1]);
        split(*scroll_p + 1, sht[*scroll_p]);
        split(*scroll_p + 2, sht[*scroll_p + 1]);
        scrollable(true, true);
        break;
    case 2:
        split(*scroll_p, sht[*scroll_p - 1]);
        split(*scroll_p + 1, sht[*scroll_p]);
        split(*scroll_p + 2, sht[*scroll_p + 1]);
        scrollable(true, true);
        break;
    case 3:
        split(*scroll_p, sht[*scroll_p - 1]);
        split(*scroll_p + 1, sht[*scroll_p]);
        split(*scroll_p + 2, sht[*scroll_p + 1]);
        scrollable(true, true);
        break;
    case 4:
        split(*scroll_p, sht[*scroll_p - 1]);
        split(*scroll_p + 1, sht[*scroll_p]);
        split(*scroll_p + 2, sht[*scroll_p + 1]);
        scrollable(true, true);
        break;
    case 5:
        split(*scroll_p, sht[*scroll_p - 1]);
        split(*scroll_p + 1, sht[*scroll_p]);
        split(*scroll_p + 2, sht[*scroll_p + 1]);
        scrollable(true, true);
        break;
    case 6:
        split(*scroll_p, sht[*scroll_p - 1]);
        split(*scroll_p + 1, sht[*scroll_p]);
        split(*scroll_p + 2, sht[*scroll_p + 1]);
        scrollable(true, false);
        break;
    default:
        scrollable(false, false);
        break;
    }
}
void LCD_UI::detail_logic()
{
    // uint8_t *scroll_p = &_scroll_position[0];
    uint8_t *scroll_r = &_scroll_prev[0];
    if (BACK_S)
    {
        _page = DASH_BOARD;
    }
    else if (SELECT_S)
    {
        _page = SETTING;
    }
    else if (DOWN_S)
    {
        _scroll_position[0]++;
    }
    else if (UP_S)
    {
        _scroll_position[0] = _scroll_position[0] == 0 ? 6 : _scroll_position[0] - 1;
    }
    if (_scroll_position[0] != *scroll_r)
    {
        detail_scroll();
    }
}
void LCD_UI::detail_refesh(void)
{
    uint8_t *scroll_p = &_scroll_position[0];
    if (*scroll_p != 0)
        lcd.gotoXY(0, 1);
    else
        lcd.gotoXY(0, 2);
    switch (*scroll_p)
    {
    case 0:
        split(0, sht[*scroll_p]);
        split(0, sht[*scroll_p + 1]);
        scrollable(false, true);
        break;
    case 1:
        split(0, sht[*scroll_p - 1]);
        split(0, sht[*scroll_p]);
        split(0, sht[*scroll_p + 1]);
        scrollable(true, true);
        break;
    case 2:
        split(0, sht[*scroll_p - 1]);
        split(0, sht[*scroll_p]);
        split(0, sht[*scroll_p + 1]);
        scrollable(true, true);
        break;
    case 3:
        split(0, sht[*scroll_p - 1]);
        split(0, sht[*scroll_p]);
        split(0, sht[*scroll_p + 1]);
        scrollable(true, true);
        break;
    case 4:
        split(0, sht[*scroll_p - 1]);
        split(0, sht[*scroll_p]);
        split(0, sht[*scroll_p + 1]);
        scrollable(true, true);
        break;
    case 5:
        split(0, sht[*scroll_p - 1]);
        split(0, sht[*scroll_p]);
        split(0, sht[*scroll_p + 1]);
        scrollable(true, true);
        break;
    case 6:
        split(0, sht[*scroll_p - 1]);
        split(0, sht[*scroll_p]);
        split(0, sht[*scroll_p + 1]);
        scrollable(true, false);
        break;
    default:
        break;
    }
}

void LCD_UI::setting_page()
{
    //Save only when in Output Setting.
    if ((_n_page != ST_INFO) && (_n_page != ST_VIEW))
        saveOutputSetting(setting);
    // readOutputSetting(setting);
    _n_page = _page;
    lcd.clear();
    lcd.write(1);
    lcd.printfCenter("SETTING");
    lcd.nextline(true);
    if (_scroll_position[1] < 3)
    {
        option("Input", true);
        option("Output", true);
        option("Time", true);
        optional_view(_scroll_position[1]);
    }
    else
    {
        option("Update", true);
        option("View", true);
        option("Info", true);
        optional_view(_scroll_position[1]);
    }
}
void LCD_UI::setting_scroll()
{
    uint8_t *scroll_p = &_scroll_position[1];
    uint8_t *scroll_r = &_scroll_prev[1];
    *scroll_p = *scroll_p > 5 ? 0 : *scroll_p;
    lcd.gotoXY(0, 1);
    switch (*scroll_p)
    {
    case 0:
        if (*scroll_r != 1)
        {
            option("Input", true);
            option("Output", true);
            option("Time", true);
        }
        optional_view(*scroll_p);
        break;
    case 1:
        optional_view(*scroll_p);
        break;
    case 2:
        if (*scroll_r != 1)
        {
            option("Input", true);
            option("Output", true);
            option("Time", true);
        }
        optional_view(*scroll_p);
        break;
    case 3:
        if (*scroll_r != 4)
        {
            option("Update", true);
            option("View", true);
            option("Info", true);
        }
        optional_view(*scroll_p);
        break;
    case 4:
        optional_view(*scroll_p);
        break;
    case 5:
        if (*scroll_r != 4)
        {
            option("Update", true);
            option("View", true);
            option("Info", true);
        }
        optional_view(*scroll_p);
        break;
    default:
        break;
    }
    *scroll_r = *scroll_p;
}
void LCD_UI::setting_logic()
{
    uint8_t *scroll_p = &_scroll_position[1];
    uint8_t *scroll_r = &_scroll_prev[1];
    if (BACK_S)
    {
        _page = DETAIL;
    }
    else if (SELECT_S)
    {
        if (*scroll_p == 0)
        {
            _page = ST_INPUT;
        }
        else if (*scroll_p == 1)
        {
            _page = ST_OUTPUT;
        }
        else if (*scroll_p == 2)
        {
            _page = ST_TIME;
        }
        else if (*scroll_p == 3)
        {
            _page = ST_UPDATE;
        }
        else if (*scroll_p == 4)
        {
            _page = ST_VIEW;
        }
        else if (*scroll_p == 5)
        {
            _page = ST_INFO;
        }
    }
    else if (DOWN_S)
    {
        *scroll_p = *scroll_p > 8 ? 0 : *scroll_p + 1;
    }
    else if (UP_S)
    {
        *scroll_p = *scroll_p == 0 ? 5 : *scroll_p - 1;
    }
    if (*scroll_p != *scroll_r)
    {
        setting_scroll();
    }
}

void LCD_UI::st_input_page()
{
    _n_page = _page;
    _scroll_position[2] = _scroll_position[2] > 1 ? 0 : _scroll_position[2];
    _scroll_prev[2] = _scroll_position[2];
    lcd.clear();
    lcd.write(1);
    lcd.printfCenter("VIEW");
    lcd.nextline(true);
    option("Temp factor", true);
    option("Humi factor", true);
    scrollable(false, false);
    optional_view2(_scroll_position[2]);
}
void LCD_UI::st_input_scroll()
{
    uint8_t *scroll_p = &_scroll_position[2];
    uint8_t *scroll_r = &_scroll_prev[2];
    lcd.gotoXY(0, 1);
    switch (_scroll_position[2])
    {
    case 0:
        optional_view2(*scroll_p);
        break;
    case 1:
        optional_view2(*scroll_p);
        break;
    default:
        *scroll_p = 0;
        optional_view2(*scroll_p);
        break;
    }
    *scroll_r = *scroll_p;
}
void LCD_UI::st_input_logic()
{
    uint8_t *scroll_p = &_scroll_position[2];
    uint8_t *scroll_r = &_scroll_prev[2];
    if (BACK_S)
    {
        _page = SETTING;
    }
    else if (SELECT_S)
    {
        if (*scroll_p == 0)
        {
            _page = ST_TEMP_FACTOR;
        }
        else if (*scroll_p == 1)
        {
            _page = ST_HUMI_FACTOR;
        }
    }
    else if (DOWN_S)
    {
        *scroll_p = *scroll_p > 1 ? 0 : *scroll_p + 1; // = max case
    }
    else if (UP_S)
    {
        *scroll_p = *scroll_p == 0 ? 1 : *scroll_p - 1;
    }
    if (*scroll_p != *scroll_r)
    {
        st_input_scroll();
    }
}

void LCD_UI::st_input_humi_factor_page()
{
    readFactorSetting(setting);
    uint8_t *scroll_p = &_scroll_position[3];
    uint8_t *_factor = setting->input->h_factor;
    _n_page = _page;
    lcd.clear();
    lcd.write(1);
    lcd.printfCenter("Temp Factor");
    scrollable(true, true);
    lcd.nextline(true);
    if (*scroll_p < 3)
    {
        option("HF0", _factor[0], true);
        option("HF1", _factor[1], true);
        option("HF2", _factor[2], true);
        optional_view(*scroll_p);
    }
    else if (*scroll_p < 6)
    {
        option("HF3", _factor[3], true);
        option("HF4", _factor[4], true);
        option("HF5", _factor[5], true);
        optional_view(*scroll_p);
    }
    else
    {
        option("HF6", _factor[6], true);
        option("HF7", _factor[7], true);
        optional_view2(*scroll_p - 5);
    }
}
void LCD_UI::st_input_humi_factor_scroll()
{
    uint8_t *scroll_p = &_scroll_position[3];
    uint8_t *scroll_r = &_scroll_prev[3];
    uint8_t *_factor = setting->input->h_factor;
    *scroll_p = *scroll_p > 7 ? 0 : *scroll_p;
    lcd.gotoXY(0, 1);
    switch (*scroll_p)
    {
    case 0:
        if (*scroll_r != 1)
        {
            option("HF0", _factor[0], true);
            option("HF1", _factor[1], true);
            option("HF2", _factor[2], true);
        }
        optional_view(*scroll_p);
        break;
    case 1:
        optional_view(*scroll_p);
        break;
    case 2:
        if (*scroll_r != 1)
        {
            option("HF0", _factor[0], true);
            option("HF1", _factor[1], true);
            option("HF2", _factor[2], true);
        }
        optional_view(*scroll_p);
        break;
    case 3:
        if (*scroll_r != 4)
        {
            option("HF3", _factor[3], true);
            option("HF4", _factor[4], true);
            option("HF5", _factor[5], true);
        }
        optional_view(*scroll_p);
        break;
    case 4:
        optional_view(*scroll_p);
        break;
    case 5:
        if (*scroll_r != 4)
        {
            option("HF3", _factor[3], true);
            option("HF4", _factor[4], true);
            option("HF5", _factor[5], true);
        }
        optional_view(*scroll_p);
        break;
    case 6:
        if (*scroll_r != 7)
        {
            option("HF6", _factor[6], true);
            option("HF7", _factor[7], true);
            lcd.clearRow();
        }
        optional_view2(*scroll_p - 6);
        break;
    case 7:
        if (*scroll_r != 6)
        {
            option("HF6", _factor[6], true);
            option("HF7", _factor[7], true);
            lcd.clearRow();
        }
        optional_view2(*scroll_p - 6);
        break;
    default:
        break;
    }
    *scroll_r = *scroll_p;
}
void LCD_UI::st_input_humi_factor_logic()
{
    uint8_t *scroll_p = &_scroll_position[3];
    uint8_t *scroll_r = &_scroll_prev[3];
    uint8_t *_factor = setting->input->h_factor;
    if (BACK_S && !on_change)
    {
        _page = ST_INPUT;
    }
    else if (SELECT_S)
    {
        if (*scroll_p == 0)
        {
            change_handle_u8(_factor, *scroll_p);
        }
        else if (*scroll_p == 1)
        {
            change_handle_u8(_factor, *scroll_p);
        }
        else if (*scroll_p == 2)
        {
            change_handle_u8(_factor, *scroll_p);
        }
        else if (*scroll_p == 3)
        {
            change_handle_u8(_factor, *scroll_p);
        }
        else if (*scroll_p == 4)
        {
            change_handle_u8(_factor, *scroll_p);
        }
        else if (*scroll_p == 5)
        {
            change_handle_u8(_factor, *scroll_p);
        }
        else if (*scroll_p == 6)
        {
            change_handle_u8(_factor, *scroll_p);
        }
        else if (*scroll_p == 7)
        {
            change_handle_u8(_factor, *scroll_p);
        }
    }
    else if (DOWN_S && !on_change)
    {
        *scroll_p = *scroll_p >= 7 ? 0 : *scroll_p + 1;
    }
    else if (UP_S && !on_change)
    {
        *scroll_p = *scroll_p == 0 ? 7 : *scroll_p - 1;
    }
    if (on_change)
    {
        st_factor_onchange(scroll_p);
    }
    else if (*scroll_p != *scroll_r)
    {
        Serial.println("Scroll out!!");
        st_input_humi_factor_scroll();
    }
}
void LCD_UI::st_input_temp_factor_page()
{
    readFactorSetting(setting);
    uint8_t *scroll_p = &_scroll_position[3];
    *scroll_p = *scroll_p > 7 ? 0 : *scroll_p;
    uint8_t *_factor = setting->input->t_factor;
    _n_page = _page;
    lcd.clear();
    lcd.write(1);
    lcd.printfCenter("Temp Factor");
    scrollable(true, true);
    lcd.nextline(true);
    if (*scroll_p < 3)
    {
        option("TF0", _factor[0], true);
        option("TF1", _factor[1], true);
        option("TF2", _factor[2], true);
        optional_view(*scroll_p);
    }
    else if (*scroll_p < 6)
    {
        option("TF3", _factor[3], true);
        option("TF4", _factor[4], true);
        option("TF5", _factor[5], true);
        optional_view(*scroll_p);
    }
    else
    {
        option("TF6", _factor[6], true);
        option("TF7", _factor[7], true);
        optional_view2(*scroll_p - 5);
    }
}
void LCD_UI::st_input_temp_factor_scroll()
{
    uint8_t *scroll_p = &_scroll_position[3];
    uint8_t *scroll_r = &_scroll_prev[3];
    uint8_t *_factor = setting->input->t_factor;
    *scroll_p = *scroll_p > 7 ? 0 : *scroll_p;
    lcd.gotoXY(0, 1);
    switch (*scroll_p)
    {
    case 0:
        if (*scroll_r != 1)
        {
            option("TF0", _factor[0], true);
            option("TF1", _factor[1], true);
            option("TF2", _factor[2], true);
        }
        optional_view(*scroll_p);
        break;
    case 1:
        optional_view(*scroll_p);
        break;
    case 2:
        if (*scroll_r != 1)
        {
            option("TF0", _factor[0], true);
            option("TF1", _factor[1], true);
            option("TF2", _factor[2], true);
        }
        optional_view(*scroll_p);
        break;
    case 3:
        if (*scroll_r != 4)
        {
            option("TF3", _factor[3], true);
            option("TF4", _factor[4], true);
            option("TF5", _factor[5], true);
        }
        optional_view(*scroll_p);
        break;
    case 4:
        optional_view(*scroll_p);
        break;
    case 5:
        if (*scroll_r != 4)
        {
            option("TF3", _factor[3], true);
            option("TF4", _factor[4], true);
            option("TF5", _factor[5], true);
        }
        optional_view(*scroll_p);
        break;
    case 6:
        if (*scroll_r != 7)
        {
            option("TF6", _factor[6], true);
            option("TF7", _factor[7], true);
            lcd.clearRow();
        }
        optional_view2(*scroll_p - 6);
        break;
    case 7:
        if (*scroll_r != 6)
        {
            option("TF6", _factor[6], true);
            option("TF7", _factor[7], true);
            lcd.clearRow();
        }
        optional_view2(*scroll_p - 6);
        break;
    default:
        break;
    }
    *scroll_r = *scroll_p;
}
void LCD_UI::st_input_temp_factor_logic()
{
    uint8_t *scroll_p = &_scroll_position[3];
    uint8_t *scroll_r = &_scroll_prev[3];
    uint8_t *_factor = setting->input->h_factor;
    if (BACK_S && !on_change)
    {
        _page = ST_INPUT;
    }
    else if (SELECT_S)
    {
        if (*scroll_p == 0)
        {
            change_handle_u8(_factor, *scroll_p);
        }
        else if (*scroll_p == 1)
        {
            change_handle_u8(_factor, *scroll_p);
        }
        else if (*scroll_p == 2)
        {
            change_handle_u8(_factor, *scroll_p);
        }
        else if (*scroll_p == 3)
        {
            change_handle_u8(_factor, *scroll_p);
        }
        else if (*scroll_p == 4)
        {
            change_handle_u8(_factor, *scroll_p);
        }
        else if (*scroll_p == 5)
        {
            change_handle_u8(_factor, *scroll_p);
        }
        else if (*scroll_p == 6)
        {
            change_handle_u8(_factor, *scroll_p);
        }
        else if (*scroll_p == 7)
        {
            change_handle_u8(_factor, *scroll_p);
        }
    }
    else if (DOWN_S && !on_change)
    {
        *scroll_p = *scroll_p >= 7 ? 0 : *scroll_p + 1;
    }
    else if (UP_S && !on_change)
    {
        *scroll_p = *scroll_p == 0 ? 7 : *scroll_p - 1;
    }
    if (on_change)
    {
        st_factor_onchange(scroll_p);
    }
    else if (*scroll_p != *scroll_r)
    {
        Serial.println("Scroll out!!");
        st_input_temp_factor_scroll();
    }
}
void LCD_UI::st_factor_onchange(uint8_t *scroll_p)
{
    // Change the temperature or humitidity.
    if (_page == ST_TEMP_FACTOR)
    {
        if (UP_S)
        {
            change_value(true, &setting->input->t_factor[*scroll_p], 10, 0);
        }
        else if (DOWN_S)
        {
            change_value(false, &setting->input->t_factor[*scroll_p], 10, 0);
        }
        else if (BACK_S)
        {
            setting->input->t_factor[*scroll_p] = pre_val_u8;
            on_change = 0;
            _n_page = RELOAD_DUMY;
        }
    }
    else
    {
        if (UP_S)
        {
            change_value(true, &setting->input->h_factor[*scroll_p], 10, 0);
        }
        else if (DOWN_S)
        {
            change_value(false, &setting->input->h_factor[*scroll_p], 10, 0);
        }
        else if (BACK_S)
        {
            setting->input->h_factor[*scroll_p] = pre_val_u8;
            on_change = 0;
            _n_page = RELOAD_DUMY;
        }
    }
}

void LCD_UI::st_output_page()
{
    uint8_t *scroll_p = &_scroll_position[2];
    uint8_t *scroll_r = &_scroll_prev[2];
    *scroll_p = *scroll_p >= 5 ? 0 : *scroll_p;
    *scroll_r = *scroll_p;

    lcd.clear();
    lcd.write(1);
    lcd.printfCenter("OUTPUT");
    scrollable(true, true);
    lcd.nextline(true);
    if ((_n_page == ST_threshold) || (_n_page == ST_TIME_LIST))
    {
        on_change = 1;
        st_output_onchange_handle(*scroll_p);
    }
    else
    {
        if (*scroll_p < 3)
        {
            optionCustom("Relay 1", CustomJSON, setting->output->relay_1.type);
            optionCustom("Relay 2", CustomJSON, setting->output->relay_2.type);
            optionCustom("Output 0", CustomJSON, setting->output->out0.type);
            optional_view(*scroll_p);
        }
        else
        {
            optionCustom("Output 0", CustomJSON, setting->output->out0.type);
            optionCustom("Output 1", CustomJSON, setting->output->out1.type);
            optionCustom("Output 2", CustomJSON, setting->output->out2.type);
            // option("Output 0", true);
            // option("Output 1", true);
            // option("Output 2", true);
            optional_view(*scroll_p);
        }
    }
    _n_page = _page;
}
void LCD_UI::st_output_scroll()
{
    uint8_t *scroll_p = &_scroll_position[2];
    uint8_t *scroll_r = &_scroll_prev[2];
    *scroll_p = *scroll_p > 4 ? 0 : *scroll_p; // = case max
    lcd.gotoXY(0, 1);
    switch (*scroll_p)
    {
    case 0:
        if (*scroll_r == 4)
        {
            optionCustom("Relay 1", CustomJSON, setting->output->relay_1.type);
            optionCustom("Relay 2", CustomJSON, setting->output->relay_2.type);
            optionCustom("Output 0", CustomJSON, setting->output->out0.type);
        }
        optional_view(*scroll_p);
        break;
    case 1:
        if (*scroll_r != 0)
        {
            optionCustom("Relay 1", CustomJSON, setting->output->relay_1.type);
            optionCustom("Relay 2", CustomJSON, setting->output->relay_2.type);
            optionCustom("Output 0", CustomJSON, setting->output->out0.type);
        }
        optional_view(*scroll_p);
        break;
    case 2:
        if (*scroll_r == 1)
            optional_view(*scroll_p);
        else
            optional_view(0);
        break;
    case 3:
        optionCustom("Output 0", CustomJSON, setting->output->out0.type);
        optionCustom("Output 1", CustomJSON, setting->output->out1.type);
        optionCustom("Output 2", CustomJSON, setting->output->out2.type);
        optional_view(*scroll_p + 1);
        break;
    case 4:
        optional_view(*scroll_p + 1);
        break;
    default:
        break;
    }

    *scroll_r = *scroll_p;
}
void LCD_UI::st_output_logic()
{
    uint8_t *scroll_p = &_scroll_position[2];
    uint8_t *scroll_r = &_scroll_prev[2];
    if (BACK_S && !on_change)
    {
        _page = SETTING;
    }
    else if (SELECT_S && !on_change)
    {
        on_change = 1;
        //Load the Detail Page.
        st_output_onchange_handle(*scroll_p);
    }
    else if (DOWN_S && !on_change)
    {
        *scroll_p = *scroll_p > 7 ? 0 : *scroll_p + 1;
    }
    else if (UP_S && !on_change)
    {
        *scroll_p = *scroll_p == 0 ? 7 : *scroll_p - 1;
    }

    //Logic handle each Output setting.
    else if (on_change && !on_change_current)
    {
        if (BACK_S)
        {
            _scroll_position[9] = _scroll_prev[9] = 0;
            on_change = 0;
            _n_page = RELOAD_DUMY;
        }
        else if (UP_S)
        {
            _scroll_position[9] = _scroll_position[9] > 0 ? _scroll_position[9] - 1 : 3;
            Serial.println("[detail]Scroll up!");
        }
        else if (DOWN_S)
        {
            _scroll_position[9] = _scroll_position[9] < 3 ? _scroll_position[9] + 1 : 0;
            Serial.println("[detail]Scroll down!");
        }
        else if (SELECT_S)
        {
            //Repose to choose option of *scroll_p Detail
            //Set the _chooseOutput
            output_setting_onchange_logic(*scroll_p);
        }

        //Scroll logic for option in Detail
        if (_scroll_position[9] != _scroll_prev[9])
        {
            // Scroll handle if not Relay 1
            if (*scroll_p != 0)
            {
                Serial.println("Scroll another");
                _scroll_position[9] = _scroll_position[9] > 2 ? 0 : _scroll_position[9];
                if ((_scroll_position[9] == 0) && (_scroll_prev[9] == 2))
                    lcd.previousline(true);
                optional_view(_scroll_position[9]);
            }
            //Scroll handle if Relay 1
            else
            {
                Serial.println("Scroll RL1");
                if (((_scroll_position[9] == 3) && (_scroll_prev[9] == 2)) || ((_scroll_position[9] == 3) && (_scroll_prev[9] == 0)))
                {
                    lcd.gotoXY(0, 1);
                    option("Cut off", setting->output->relay_1.current, true);
                    optional_view1();
                }
                else
                {
                    optional_view(_scroll_position[9]);
                }
                if (((_scroll_position[9] == 0) && (_scroll_prev[9] == 3)) || ((_scroll_position[9] == 2) && (_scroll_prev[9] == 3)))
                {
                    //Show 3 option 0-2
                    st_output_onchange_handle(*scroll_p);
                    optional_view(_scroll_position[9]);
                }
            }

            _scroll_prev[9] = _scroll_position[9];
        }
    }
    else if (on_change && on_change_current)
    {
        if (BACK_S)
        {
            lcd.cursor(setting->ui.cursor);
            lcd.gotoXY(0, 1);
            option("Cut off", _chooseOutput->current, true);
            optional_view1();
            // saveOutputSetting(setting);
            // readOutputSetting(setting);
            on_change_current = 0;
        }
        else if (SELECT_S)
        {
            change_p--;
            if (change_p < 2)
            {
                lcd.cursor(setting->ui.cursor);
                lcd.gotoXY(0, 1);
                option("Cut off", _chooseOutput->current, true);
                optional_view1();
                // saveOutputSetting(setting);
                // readOutputSetting(setting);
                on_change_current = 0;
            }
        }
        else if (UP_S)
        {
            //Incr the value
            change_value(true, (float *)&_chooseOutput->current, 5.0f, 0.0f, change_p);
            Serial.printf("%.02f\r\n", setting->output->relay_1.current);
        }
        else if (DOWN_S)
        {
            change_value(false, (float *)&_chooseOutput->current, 5.0f, 0.0f, change_p);
            Serial.printf("%.02f\r\n", setting->output->relay_1.current);
        }
    }
    if (*scroll_p != *scroll_r)
    {
        Serial.println("Scroll out!!");
        st_output_scroll();
    }
}
//Load the Detail Page. (Run once)
void LCD_UI::st_output_onchange_handle(uint8_t scroll_num)
{
    // readOutputSetting(setting);
    switch (scroll_num)
    {
    case 0:
        output_setting_detail_page(&setting->output->relay_1, "RELAY 1");
        break;
    case 1:
        output_setting_detail_page(&setting->output->relay_2, "RELAY 2");
        break;
    case 2:
        output_setting_detail_page(&setting->output->out0, "OUT 0");
        break;
    case 3:
        output_setting_detail_page(&setting->output->out1, "OUT 1");
        break;
    case 4:
        output_setting_detail_page(&setting->output->out2, "OUT 2");
        break;
    default:
        break;
    }
}
//Repose to choose option in Detail.
void LCD_UI::output_setting_onchange_logic(uint8_t scroll_num)
{
    uint8_t *scroll_p = &_scroll_position[9];
    if (scroll_num != 0)
    {
        _scroll_position[9] = _scroll_position[9] < 5 ? _scroll_position[9] : 0;
    }
    switch (scroll_num)
    {
    case 0:
        _chooseOutput = &setting->output->relay_1;
        output_setting_detail_onchange_logic(*scroll_p, _chooseOutput, &output_state[0]);
        break;
    case 1:
        _chooseOutput = &setting->output->relay_2;
        output_setting_detail_onchange_logic(*scroll_p, _chooseOutput, &output_state[1]);
        break;
    case 2:
        _chooseOutput = &setting->output->out0;
        output_setting_detail_onchange_logic(*scroll_p, _chooseOutput, &output_state[2]);
        break;
    case 3:
        _chooseOutput = &setting->output->out1;
        output_setting_detail_onchange_logic(*scroll_p, _chooseOutput, &output_state[3]);
        break;
    case 4:
        _chooseOutput = &setting->output->out2;
        output_setting_detail_onchange_logic(*scroll_p, _chooseOutput, &output_state[4]);
        break;
    default:

        break;
    }
}

//Do the task has been choose in Detail
void LCD_UI::output_setting_detail_onchange_logic(uint8_t scroll_num, st_output_config_t *out, bool *_output_state)
{
    switch (scroll_num)
    {

    //Go to the Threshold option page. When back will call Output Page -> the Detail Page once.
    case 0:

        selectThresholdOutput(out);
        break;

    //Change the Output type
    case 1:

        if ((out->type == 0) || (out->type == 3))
        {
            out->isHeater = out->isHeater != 0U ? 0U : 1U;
            optionCustom("Output", "Heater", "Cooler", out->isHeater);
        }
        else if ((out->type == 1) || (out->type == 4))
        {
            out->isHeater = out->isHeater != 0U ? 0U : 1U;
            optionCustom("Output", "Steam", "Dryer", out->isHeater);
        }
        else if ((out->type == 2) || (out->type == 5))
        {
            _page = ST_TIME_LIST;
            // uint8_t _numOftheOnTime = 0;
            // for (uint8_t i = 0; i < 6; i++)
            // {
            //     _numOftheOnTime = out->time_state[i].isUsed == 1 ? _numOftheOnTime + 1 : _numOftheOnTime;
            // }
            // option("Used", _numOftheOnTime, true);
        }
        else if (out->type == 6)
        {
            *_output_state = !*_output_state;
            optionCustom("Output", "ON", "OFF", *_output_state);
        }
        // else
        //     option("Cut off", out->current, true);
        // Serial.printf("[output_setting_detail_onchange_logic] %s\r\n", out->isHeater ? "change to Steam or Heater" : "change to Dryer or Cooler");
        optional_view(scroll_num);
        // saveOutputSetting(setting);
        break;

    //Change the Denpen on
    case 2:
        if (out == &setting->output->relay_1)
            out->type = out->type < 6U ? out->type + 1 : 0U;
        else
        {
            out->type = out->type < 3U ? out->type + 1 : 0U;
            out->type = out->type == 3U ? 6 : out->type;
        }
        lcd.previousline(true);
        if ((out->type == 0) || (out->type == 3))
            optionCustom("Output", "Heater", "Cooler", out->isHeater);
        else if ((out->type == 1) || (out->type == 4))
            optionCustom("Output", "Steam", "Dryer", out->isHeater);
        else if ((out->type == 2) || (out->type == 5))
        {
            uint8_t _numOftheOnTime = 0;
            for (uint8_t i = 0; i < 6; i++)
            {
                _numOftheOnTime = out->time_state[i].isUsed == 1 ? _numOftheOnTime + 1 : _numOftheOnTime;
            }
            option("Time Period", _numOftheOnTime, true);
        }
        else if (out->type == 6)
        {
            optionCustom("Output", "ON", "OFF", *_output_state);
        }
        // else
        //     option("Cut off", out->current, true);

        optionCustom("Depend", CustomJSON, out->type);
        Serial.print("[output_setting_detail_onchange_logic] ");
        Serial.println(CustomJSON[out->type]);
        optional_view(scroll_num);
        // saveOutputSetting(setting);
        break;
    //Change the Current value
    case 3:
        change_p = change_p < 2 ? 3 : change_p;
        lcd.gotoend();
        lcd.cursor(true);
        on_change_current = 1;
        break;
    //Turn on or off the current protection.
    case 4:

        break;
    }
}
void LCD_UI::output_setting_detail_page(st_output_config_t *out, const char *tilte)
{
    lcd.clear();
    lcd.write(1);
    lcd.printfCenter(tilte);
    scrollable(true, true);
    lcd.nextline(true);
    if (out != &setting->output->relay_1)
    {
        option("Thresholds", true);
        if ((out->type == 0) || (out->type == 3))
            optionCustom("Output", "Heater", "Cooler", out->isHeater);
        else if ((out->type == 1) || (out->type == 4))
            optionCustom("Output", "Steam", "Dryer", out->isHeater);
        else if ((out->type == 2) || (out->type == 5))
        {
            uint8_t _numOftheOnTime = 0;
            for (uint8_t i = 0; i < 6; i++)
            {
                _numOftheOnTime = out->time_state[i].isUsed == 1 ? _numOftheOnTime + 1 : _numOftheOnTime;
            }
            option("Time Period", _numOftheOnTime, true);
        }
        else if (out->type == 6)
        {
            optionCustom("Output", "ON", "OFF", 1);
        }
        optionCustom("Depend", CustomJSON, out->type);
        optional_view(_scroll_position[9]);
    }
    else
    {
        option("Thresholds", true);
        if ((out->type == 0) || (out->type == 3))
            optionCustom("Output", "Heater", "Cooler", out->isHeater);
        else if ((out->type == 1) || (out->type == 4))
            optionCustom("Output", "Steam", "Dryer", out->isHeater);
        else if ((out->type == 2) || (out->type == 5))
        {
            uint8_t _numOftheOnTime = 0;
            for (uint8_t i = 0; i < 6; i++)
            {
                _numOftheOnTime = out->time_state[i].isUsed == 1 ? _numOftheOnTime + 1 : _numOftheOnTime;
            }
            option("Time Period", _numOftheOnTime, true);
        }
        else if (out->type == 6)
        {
            optionCustom("Output", "ON", "OFF", 1);
        }
        optionCustom("Depend", CustomJSON, out->type);
        optional_view(_scroll_position[9]);
    }
}
void LCD_UI::selectThresholdOutput(st_output_config_t *this_output)
{
    Serial.printf("[selectThresholdOutput] Temp_H %5.02f; Temp_L %5.02f\r\n", this_output->threshold.temp_h, this_output->threshold.temp_l);
    _page = ST_threshold;
}
void LCD_UI::st_threshold_page()
{
    // Uncomment this for testing purpose
    // readOutputSetting(setting);
    change_p = 0;
    _n_page = _page;
    _scroll_position[3] = _scroll_prev[3] = 0;
    lcd.clear();
    lcd.write(1);
    lcd.printfCenter("Threshold");
    lcd.nextline(true);
    if (_scroll_position[3] < 3)
    {
        option("Temp H", _chooseOutput->threshold.temp_h, true);
        option("Temp L", _chooseOutput->threshold.temp_l, true);
        option("Humi H", _chooseOutput->threshold.humi_h, true);
        optional_view(_scroll_position[3]);
    }
    else
    {
        option("Temp L", _chooseOutput->threshold.temp_l, true);
        option("Humi H", _chooseOutput->threshold.humi_h, true);
        option("Humi L", _chooseOutput->threshold.humi_l, true);
        optional_view(2);
    }
    // option("Humi OFF", (float)85.5, true);
    // scrollable(false, false);
}
void LCD_UI::st_threshold_scroll()
{
    uint8_t *scroll_p = &_scroll_position[3];
    uint8_t *scroll_r = &_scroll_prev[3];
    *scroll_p = *scroll_p > 3 ? 0 : *scroll_p; // = case max

    lcd.gotoXY(0, 1);
    switch (*scroll_p)
    {
    case 0:
        if ((*scroll_r == 1) || (*scroll_r == 3))
        {
            option("Temp H", _chooseOutput->threshold.temp_h, true);
            option("Temp L", _chooseOutput->threshold.temp_l, true);
            option("Humi H", _chooseOutput->threshold.humi_h, true);
        }
        optional_view(*scroll_p);
        break;
    case 1:
        if (*scroll_r == 2)
            optional_view(*scroll_p - 1);
        else
            optional_view(*scroll_p);
        break;
    case 2:
        if (*scroll_r == 3)
            optional_view(*scroll_p - 1);
        else
            optional_view(*scroll_p);
        break;
    case 3:
        option("Temp L", _chooseOutput->threshold.temp_l, true);
        option("Humi H", _chooseOutput->threshold.humi_h, true);
        option("Humi L", _chooseOutput->threshold.humi_l, true);
        optional_view(*scroll_p - 1);
        break;
    default:
        break;
    }

    *scroll_r = *scroll_p;
}
void LCD_UI::st_threshold_logic()
{
    uint8_t *scroll_p = &_scroll_position[3];
    uint8_t *scroll_r = &_scroll_prev[3];
    if (BACK_S && !on_change_threshold)
    {
        // saveOutputSetting(setting);
        _page = ST_OUTPUT;
    }
    else if (SELECT_S)
    {
        if (*scroll_p == 0)
        {
            change_handle_float(_chooseOutput->threshold.temp_h);
        }
        else if (*scroll_p == 1)
        {
            change_handle_float(_chooseOutput->threshold.temp_l);
        }
        else if (*scroll_p == 2)
        {
            change_handle_float(_chooseOutput->threshold.humi_h);
        }
        else if (*scroll_p == 3)
        {
            change_handle_float(_chooseOutput->threshold.humi_l);
        }
    }
    else if (DOWN_S && !on_change_threshold)
    {
        *scroll_p = *scroll_p > 3 ? 0 : *scroll_p + 1; // = max case
    }
    else if (UP_S && !on_change_threshold)
    {
        *scroll_p = *scroll_p == 0 ? 0 : *scroll_p - 1;
    }

    if (*scroll_p != *scroll_r)
    {
        st_threshold_scroll();
    }

    if (on_change_threshold)
    {
        st_threshold_on_change(scroll_p);
    }
}
void LCD_UI::st_threshold_on_change(uint8_t *scroll_p)
{
    if (*scroll_p == 0)
    {
        if (UP_S)
        {
            Serial.println("[st_threshold_on_change]Up");
            change_value(true, &_chooseOutput->threshold.temp_h, 110.0, -99.99, change_p);
        }
        else if (DOWN_S)
        {
            Serial.println("[st_threshold_on_change]Down");
            change_value(false, &_chooseOutput->threshold.temp_h, 110.0, -99.99, change_p);
        }
        else if (BACK_S)
        {
            //Save EEPROM

            Serial.println("[st_threshold_on_change]Back");
            lcd.cursor(setting->ui.cursor);
            _chooseOutput->threshold.temp_h = pre_val;
            on_change_threshold = change_p = 0;
            _n_page = RELOAD_DUMY;
        }
    }
    else if (*scroll_p == 1)
    {
        if (UP_S)
        {
            change_value(true, &_chooseOutput->threshold.temp_l, 110.0, -99.99, change_p);
        }
        else if (DOWN_S)
        {
            change_value(false, &_chooseOutput->threshold.temp_l, 110.0, -99.99, change_p);
        }
        else if (BACK_S)
        {
            _chooseOutput->threshold.temp_l = pre_val;
            on_change_threshold = change_p = 0;
            _n_page = RELOAD_DUMY;
        }
    }
    else if (*scroll_p == 2)
    {
        if (UP_S)
        {
            change_value(true, &_chooseOutput->threshold.humi_h, 110.0, -99.99, change_p);
        }
        else if (DOWN_S)
        {
            change_value(false, &_chooseOutput->threshold.humi_h, 110.0, -99.99, change_p);
        }
        else if (BACK_S)
        {
            _chooseOutput->threshold.humi_h = pre_val;
            on_change_threshold = change_p = 0;
            _n_page = RELOAD_DUMY;
        }
    }
    else if (*scroll_p == 3)
    {
        if (UP_S)
        {
            change_value(true, &_chooseOutput->threshold.humi_l, 110.0, -99.99, change_p);
        }
        else if (DOWN_S)
        {
            change_value(false, &_chooseOutput->threshold.humi_l, 110.0, -99.99, change_p);
        }
        else if (BACK_S)
        {
            _chooseOutput->threshold.humi_l = pre_val;
            on_change_threshold = change_p = 0;
            _n_page = RELOAD_DUMY;
        }
    }
}

void LCD_UI::st_time_main()
{
    _n_page = _page;
    Serial.println("Time Main Page");
    _scroll_position[3] = _scroll_prev[3] = 0;
    lcd.clear();
    lcd.write(1);
    lcd.printfCenter("TIME");
    lcd.nextline(true);
}
void LCD_UI::st_time_main_scroll() {}
void LCD_UI::st_time_main_logic()
{
    if (BACK_S)
    {
        _page = SETTING;
    }
    else if (SELECT_S)
    {
    }
    else if (UP_S)
    {
    }
    else if (DOWN_S)
    {
    }
}
void LCD_UI::split(st_timer_t out)
{
    lcd.clearRow();
    if (out.isUsed)
        lcd.write(5);
    else
        lcd.write(6);
    struct tm t;
    localtime_r(&out.date_time[0], &t);
    lcd.printfdata("%02i:%02i", t.tm_hour, t.tm_min);
    lcd.printfdata("-");
    localtime_r(&out.date_time[1], &t);
    lcd.printfdata("%02i:%02i|", t.tm_hour, t.tm_min);
    lcd.gotoend();
    lcd.printfBackward(out.state == 0 ? "O I" : "I O");
    lcd.movecursor(2);
    lcd.write(0);
    lcd.gotobegin();
}
void LCD_UI::split(const char *col1, const char *col2, const char *col3, uint8_t st1, uint8_t st2, uint8_t st3)
{
    st1 = st1 > 15 ? 0 : st1;
    st2 = st2 > 15 ? 0 : st2;
    st3 = st3 > 15 ? 0 : st3;
    lcd.clearRow();
    lcd.gotoX(st1);
    lcd.printfdata(col1);
    lcd.gotoX(st2);
    lcd.printfdata(col2);
    lcd.gotoX(st3);
    lcd.printfdata(col3);
    lcd.nextline(true);
}
void LCD_UI::st_time_page()
{
    if (_n_page != ST_DETAIL_TIME)
        _scroll_position[8] = _scroll_prev[8] = 0;
    _n_page = _page;
    lcd.clear();
    lcd.write(0);
    lcd.printfdata(" 1-2");
    if (_chooseOutput == &setting->output->relay_1)
        title = "RLY1";
    else if (_chooseOutput == &setting->output->relay_2)
        title = "RLY2";
    else if (_chooseOutput == &setting->output->out0)
        title = "OUT1";
    else if (_chooseOutput == &setting->output->out1)
        title = "OUT2";
    else
        title = "OUT3";
    lcd.printfCenter(title.c_str());
    lcd.nextline(true);
    st_time_scroll();
    // split("Start|", " End |", " T", 1, 7, 13);
    // split(_chooseOutput->time_state[0]);
    // Serial.printf("Unix 1 %lu - %lu \r\n", _chooseOutput->time_state[0].date_time[0], _chooseOutput->time_state[0].date_time[1]);
    // lcd.nextline(true);

    // split(_chooseOutput->time_state[1]);
    // Serial.printf("Unix 2 %lu - %lu \r\n", _chooseOutput->time_state[1].date_time[0], _chooseOutput->time_state[1].date_time[1]);
    // lcd.nextline(true);
    // lcd.gotoend();
    // lcd.movecursor(-2);
    // lcd.printfdata("%u\4", _scroll_position[8] + 1);
    // lcd.movecursor(-1);
}
void LCD_UI::st_time_scroll()
{
    uint8_t *scroll_p = &_scroll_position[8];
    uint8_t *scroll_r = &_scroll_prev[8];
    lcd.gotoXY(0, 1);
    switch (*scroll_p)
    {
    case 0:
        split("Start ", " End |", " T", 1, 7, 13);
        split(_chooseOutput->time_state[0]);
        lcd.nextline(true);
        split(_chooseOutput->time_state[1]);
        lcd.nextline(true);
        lcd.gotoXY(2, 0);
        lcd.printfdata("1-2");
        lcd.gotoend();
        lcd.movecursor(-2);
        lcd.printfdata("%u", *scroll_p + 1);
        scrollable(false, true);
        break;
    case 1:
        split(_chooseOutput->time_state[0]);
        lcd.nextline(true);
        split(_chooseOutput->time_state[1]);
        lcd.nextline(true);
        split(_chooseOutput->time_state[2]);
        lcd.nextline(true);
        lcd.gotoXY(2, 0);
        lcd.printfdata("%u-%u", *scroll_p, *scroll_p + 2);
        lcd.gotoend();
        lcd.movecursor(-2);
        lcd.printfdata("%u", *scroll_p + 1);
        scrollable(true, true);
        break;
    case 2:
        split(_chooseOutput->time_state[1]);
        lcd.nextline(true);
        split(_chooseOutput->time_state[2]);
        lcd.nextline(true);
        split(_chooseOutput->time_state[3]);
        lcd.nextline(true);
        lcd.gotoXY(2, 0);
        lcd.printfdata("%u-%u", *scroll_p, *scroll_p + 2);
        lcd.gotoend();
        lcd.movecursor(-2);
        lcd.printfdata("%u", *scroll_p + 1);
        scrollable(true, true);
        break;
    case 3:
        split(_chooseOutput->time_state[2]);
        lcd.nextline(true);
        split(_chooseOutput->time_state[3]);
        lcd.nextline(true);
        split(_chooseOutput->time_state[4]);
        lcd.nextline(true);
        lcd.gotoXY(2, 0);
        lcd.printfdata("%u-%u", *scroll_p, *scroll_p + 2);
        lcd.gotoend();
        lcd.movecursor(-2);
        lcd.printfdata("%u", *scroll_p + 1);
        scrollable(true, true);
        break;
    case 4:
        split(_chooseOutput->time_state[3]);
        lcd.nextline(true);
        split(_chooseOutput->time_state[4]);
        lcd.nextline(true);
        split(_chooseOutput->time_state[5]);
        lcd.nextline(true);
        lcd.gotoXY(2, 0);
        lcd.printfdata("%u-%u", *scroll_p, *scroll_p + 2);
        lcd.gotoend();
        lcd.movecursor(-2);
        lcd.printfdata("%u", *scroll_p + 1);
        scrollable(true, true);
        break;
    case 5:
        // split(*scroll_p, sht[*scroll_p - 1]);
        // split(*scroll_p + 1, sht[*scroll_p]);
        // split(*scroll_p + 2, sht[*scroll_p + 1]);
        if (*scroll_r == 0)
        {
            split(_chooseOutput->time_state[3]);
            lcd.nextline(true);
            split(_chooseOutput->time_state[4]);
            lcd.nextline(true);
            split(_chooseOutput->time_state[5]);
            lcd.nextline(true);
            lcd.gotoXY(2, 0);
            lcd.printfdata("%u-%u", *scroll_p, *scroll_p + 2);
            lcd.gotoend();
            lcd.movecursor(-2);
            lcd.printfdata("%u", *scroll_p + 1);
            scrollable(true, false);
        }
        lcd.gotoXY(0, 0);
        lcd.gotoend();
        lcd.movecursor(-2);
        lcd.printfdata("%u", *scroll_p + 1);
        scrollable(true, false);
        break;
    case 6:
        // split(*scroll_p, sht[*scroll_p - 1]);
        // split(*scroll_p + 1, sht[*scroll_p]);
        // split(*scroll_p + 2, sht[*scroll_p + 1]);
        // lcd.gotoXY(0, 0);s
        break;
    default:
        // scrollable(false, false);
        break;
    }
    *scroll_r = *scroll_p;
}
void LCD_UI::st_time_logic()
{
    uint8_t *scroll_p = &_scroll_position[8];
    uint8_t *scroll_r = &_scroll_prev[8];
    if (BACK_S)
    {
        _page = ST_OUTPUT;
    }
    else if (SELECT_S)
    {
        _chooseTimer = &_chooseOutput->time_state[*scroll_p];
        _page = ST_DETAIL_TIME;
        Serial.println("Time Select press!!");
    }
    else if (DOWN_S)
    {
        *scroll_p = *scroll_p < 5 ? *scroll_p + 1 : 0;
    }
    else if (UP_S)
    {
        // _scroll_position[0] = _scroll_position[0] == 0 ? 6 : _scroll_position[0] - 1;
        *scroll_p = *scroll_p > 0 ? *scroll_p - 1 : 5;
    }
    if (*scroll_p != *scroll_r)
    {
        st_time_scroll();
        *scroll_r = *scroll_p;
    }
}

void LCD_UI::option(const char *opt, time_t tm_val)
{
    struct tm t;
    localtime_r(&tm_val, &t);
    lcd.clearRow();
    lcd.write('>');
    lcd.printfdata(opt);
    lcd.gotoend();
    lcd.printfBackward("%02i:%02i", t.tm_hour, t.tm_min);
    lcd.nextline(true);
}
void LCD_UI::st_output_time_detail_page(void)
{
    _n_page = _page;
    _scroll_position[9] = _scroll_prev[9] = 0;
    lcd.clear();
    lcd.write(0);
    title += "-T";
    title += _scroll_position[8] + 1;
    lcd.printfCenter(title.c_str());
    lcd.nextline(true);
    optionCustom("Used", "\5", "\6", _chooseTimer->isUsed);
    option("Start", _chooseTimer->date_time[0]);
    option("End", _chooseTimer->date_time[1]);
    optional_view(_scroll_position[9]);
    // optionCustom("Type", "ON>OFF", "OFF>ON", _chooseTimer->state);
}
void LCD_UI::st_output_time_detail_scroll(void)
{
    uint8_t *scroll_p = &_scroll_position[9];
    uint8_t *scroll_r = &_scroll_prev[9];
    lcd.gotoXY(0, 1);
    switch (*scroll_p)
    {
    case 0:
        if (*scroll_r != 1)
        {
            optionCustom("Used", "\5", "\6", _chooseTimer->isUsed);
            option("Start", _chooseTimer->date_time[0]);
            option("End", _chooseTimer->date_time[1]);
            scrollable(false, true);
        }
        optional_view(*scroll_p);
        break;
    case 1:
        optional_view(*scroll_p);
        break;
    case 2:
        if (*scroll_r != 1)
        {
            optionCustom("Used", "\5", "\6", _chooseTimer->isUsed);
            option("Start", _chooseTimer->date_time[0]);
            option("End", _chooseTimer->date_time[1]);
            scrollable(false, true);
        }
        optional_view(*scroll_p);
        break;
    case 3:
        optionCustom("Type", "ON>OFF", "OFF>ON", _chooseTimer->state);
        scrollable(true, false);
        optional_view1();
        break;
    default:
        // scrollable(false, false);
        break;
    }
}
void LCD_UI::st_output_time_detail_logic(void)
{
    uint8_t *scroll_p = &_scroll_position[9];
    uint8_t *scroll_r = &_scroll_prev[9];
    if (BACK_S && !on_change_time)
    {
        _page = ST_TIME_LIST;
    }
    else if (UP_S && !on_change_time)
    {
        *scroll_p = *scroll_p > 0 ? *scroll_p - 1 : 3;
    }
    else if (DOWN_S && !on_change_time)
    {
        *scroll_p = *scroll_p < 3 ? *scroll_p + 1 : 0;
    }
    else if (SELECT_S && !on_change_time)
    {
        on_change_time = 1;
    }
    if (*scroll_p != *scroll_r)
    {
        st_output_time_detail_scroll();
        *scroll_r = *scroll_p;
    }
    if (on_change_time)
    {

        if (BACK_S)
        {
            on_change_time = 0;
            isHourChange = 0;
            lcd.cursor(setting->ui.cursor);
            lcd.gotobegin();
        }
        else if (SELECT_S)
            st_output_time_detail_on_change_handle(*scroll_p);
        else if (UP_S)
        {
            if (*scroll_p == 1)
            {
                change_time_detail(&_chooseTimer->date_time[0], true);
            }
            else if (*scroll_p == 2)
            {
                change_time_detail(&_chooseTimer->date_time[1], true);
            }
        }
        else if (DOWN_S)
        {
            if (*scroll_p == 1)
            {
                change_time_detail(&_chooseTimer->date_time[0], false);
            }
            else if (*scroll_p == 2)
            {
                change_time_detail(&_chooseTimer->date_time[1], false);
            }
        }
    }
}
void LCD_UI::st_output_time_detail_on_change_handle(uint8_t select_num)
{
    switch (select_num)
    {
    case 0:
        lcd.gotoend();
        _chooseTimer->isUsed = _chooseTimer->isUsed == 0 ? 1 : 0;
        lcd.writenotmove(_chooseTimer->isUsed == 1 ? 5 : 6);
        lcd.gotobegin();
        on_change_time = 0;
        break;
    case 1:
        isHourChange++;
        lcd.cursor(true);
        if (isHourChange >= 4)
        {
            on_change_time = isHourChange = 0;
            lcd.cursor(setting->ui.cursor);
            lcd.gotobegin();
        }
        else
        {
            Serial.printf("Enter the change time: %u \r\n", isHourChange);
            if (isHourChange == 1)
                lcd.gotoX(11 + isHourChange);
            else
                lcd.gotoX(12 + isHourChange);
        }
        break;
    case 2:
        isHourChange++;

        lcd.cursor(true);
        if (isHourChange >= 4)
        {
            on_change_time = isHourChange = 0;
            lcd.cursor(setting->ui.cursor);
            lcd.gotobegin();
        }
        else
        {
            Serial.printf("Enter the change time: %u \r\n", isHourChange);
            if (isHourChange == 1)
                lcd.gotoX(11 + isHourChange);
            else
                lcd.gotoX(12 + isHourChange);
        }
        break;
    case 3:
        lcd.gotoend();
        _chooseTimer->state = _chooseTimer->state == 0 ? 1 : 0;
        lcd.printfBackward("%s", _chooseTimer->state == 0 ? "OFF>ON" : "ON>OFF");
        lcd.gotobegin();
        on_change_time = 0;
        break;
    default:
        on_change_time = 0;
        break;
    }
}
void LCD_UI::change_time_detail(time_t *val, bool isIncrease)
{
    struct tm t, rtc_tm;
    localtime_r(val, &t);
    localtime_r(&rtc, &rtc_tm);
    t.tm_mday = rtc_tm.tm_mday;
    t.tm_mon = rtc_tm.tm_mon;
    t.tm_year = rtc_tm.tm_year;
    if (isIncrease)
    {
        if (isHourChange == 1)
            t.tm_hour = t.tm_hour < 23 ? t.tm_hour + 1 : 0;
        else if (isHourChange == 2)
            t.tm_min = t.tm_min <= 49 ? t.tm_min + 10 : t.tm_min - 50;
        else if (isHourChange == 3)
            t.tm_min = t.tm_min < 59 ? t.tm_min + 1 : 0;
    }
    else
    {
        if (isHourChange == 1)
            t.tm_hour = t.tm_hour > 0 ? t.tm_hour - 1 : 23;
        else if (isHourChange == 2)
            t.tm_min = t.tm_min >= 10 ? t.tm_min - 10 : 50 + t.tm_min;
        else if (isHourChange == 3)
            t.tm_min = t.tm_min > 0 ? t.tm_min - 1 : 59;
    }
    //Load the value
    lcd.gotoend();
    lcd.printfBackward("%02i:%02i", t.tm_hour, t.tm_min);
    if (isHourChange == 1)
        lcd.gotoX(11 + isHourChange);
    else
        lcd.gotoX(12 + isHourChange);
    *val = mktime(&t);
}

void LCD_UI::st_view_page()
{
    _n_page = _page;
    lcd.clear();
    lcd.write(1);
    lcd.printfCenter("VIEW");
    lcd.nextline(true);
    optionOF("Cursor blink", setting->ui.blink, true);
    optionOF("Cursor view", setting->ui.cursor, true);
    optionOF("LED", setting->ui.led, true);
    // scrollable(false, false);
    st_view_scroll();
}
void LCD_UI::st_view_scroll()
{
    uint8_t *scroll_p = &_scroll_position[2];
    uint8_t *scroll_r = &_scroll_prev[2];

    *scroll_p = *scroll_p > 2 ? 0 : *scroll_p;

    lcd.gotoXY(0, 1);
    switch (*scroll_p)
    {
    case 0:
        optional_view(0);
        break;
    case 1:
        optional_view(1);
        break;
    case 2:
        optional_view(2);
        break;
    // case 3:
    //     // optionOF("LED", setting->ui.led, true);
    //     // option("Opt 4", 36.6, true);
    //     // optionOF("Opt 5", true, true);
    //     optional_view(1);
    //     break;
    // case 4:
    //     scrollable(true, false);
    //     optional_view(2);
    // break;
    default:
        scrollable(false, false);
        break;
    }
    *scroll_r = *scroll_p;
}
void LCD_UI::st_view_logic()
{
    uint8_t *scroll_p = &_scroll_position[2];
    uint8_t *scroll_r = &_scroll_prev[2];
    if (BACK_S)
    {
        _page = SETTING;
    }
    else if (SELECT_S)
    {
        if (*scroll_p == 0)
        {
            setting->ui.blink = !setting->ui.blink;
            lcd.blink(setting->ui.blink);
            optionOF_change(setting->ui.blink);
        }
        else if (*scroll_p == 1)
        {
            setting->ui.cursor = !setting->ui.cursor;
            lcd.cursor(setting->ui.cursor);
            optionOF_change(setting->ui.cursor);
        }
        else if (*scroll_p == 2)
        {
            setting->ui.led = !setting->ui.led;
            lcd.led(setting->ui.led);
            optionOF_change(setting->ui.led);
        }
    }
    else if (DOWN_S)
    {
        *scroll_p = *scroll_p > 2 ? 0 : *scroll_p + 1;
    }
    else if (UP_S)
    {
        *scroll_p = *scroll_p == 0 ? 2 : *scroll_p - 1;
    }
    if (*scroll_p != *scroll_r)
    {
        st_view_scroll();
    }
}

void LCD_UI::st_info_page()
{
    _n_page = _page;
    lcd.clear();
    lcd.write(1);
    lcd.printfCenter("INFO");
    lcd.nextline(true);
    _scroll_position[3] = _scroll_position[3] >= 2 ? 0 : _scroll_position[3];
    lcd.gotoXY(0, 1);
    switch (_scroll_position[3])
    {
    case 0:
        option("Contact", true);
        option("Update", true);
        optional_view2(0);
        break;
    case 1:
        option("Contact", true);
        option("Update", true);
        optional_view2(1);
        break;
    default:
        // scrollable(false, false);
        break;
    }
}

void LCD_UI::st_info_logic()
{
    if (BACK_S)
    {
        _page = SETTING;
    }
    else if (SELECT_S)
    {
        if (_scroll_position[1] == 0)
        {
            _page = CONTACT;
        }
        else if (_scroll_position[1] == 1)
        {
            _page = YESNO;
            _page_ys = ST_INFO;
            lcd.clear();
            lcd.printfCenter("IMPORTANT!!");
            lcd.printfdata("Firmware version now is %.1f. Do you want to update?", 0.1);
        }
    }
    else if (DOWN_S)
    {
        _scroll_position[1] = _scroll_position[1] > 1 ? 0 : _scroll_position[1] + 1;
    }
    else if (UP_S)
    {
        _scroll_position[1] = _scroll_position[1] == 0 ? 1 : _scroll_position[1] - 1;
    }
    if (_scroll_position[1] != _scroll_prev[1])
    {
        _scroll_position[1] = _scroll_position[1] >= 2 ? 0 : _scroll_position[1];
        lcd.gotoXY(0, 1);
        switch (_scroll_position[1])
        {
        case 0:
            if (_scroll_prev[1] != 1)
            {
                option("Contact", true);
                option("Update", true);
            }
            optional_view2(0);
            break;
        case 1:
            optional_view2(1);
            break;
        default:
            // scrollable(false, false);
            break;
        }
        _scroll_prev[1] = _scroll_position[1];
    }
}

void LCD_UI::yesno_page()
{
    _n_page = _page;
    lcd.gotoXY(0, 3);
    optionYN("YES", "NO", false);
}

//Option logic
void LCD_UI::change_value(bool incre, float *val, float h_limit, float l_limit, uint8_t dec)
{
    if (incre)
        switch (dec)
        {
        case 0:
            lcd.gotoend();
            if (*val < h_limit - 10)
                *val = *val + 10;
            lcd.printfBackward("%s%05.2f", (*val >= 0 && *val < 100) ? " " : "", *val);
            lcd.gotoX(15 - 4 + dec);
            break;
        case 1:
            lcd.gotoend();
            if (*val < h_limit - 1)
                *val = *val + 1;
            lcd.printfBackward("%s%05.2f", (*val >= 0 && *val < 100) ? " " : "", *val);
            lcd.gotoX(15 - 4 + dec);
            break;
        case 2:
            lcd.gotoend();
            if (*val < h_limit - 0.1)
                *val = *val + 0.1;
            lcd.printfBackward("%s%05.2f", (*val >= 0 && *val < 100) ? " " : "", *val);
            lcd.gotoX(15 - 3 + dec);
            break;
        case 3:
            lcd.gotoend();
            if (*val < h_limit - 0.01)
                *val = *val + 0.01;
            lcd.printfBackward("%s%05.2f", (*val >= 0 && *val < 100) ? " " : "", *val);
            lcd.gotoX(15 - 3 + dec);
            break;
        default:
            break;
        }
    else
        switch (dec)
        {
        case 0:
            lcd.gotoend();
            if (*val > l_limit + 10)
                *val = *val - 10;
            lcd.printfBackward("%s%05.2f", (*val >= 0 && *val < 100) ? " " : "", *val);
            lcd.gotoX(15 - 4 + dec);
            break;
        case 1:
            lcd.gotoend();
            if (*val > l_limit + 1)
                *val = *val - 1;
            lcd.printfBackward("%s%05.2f", (*val >= 0 && *val < 100) ? " " : "", *val);
            lcd.gotoX(15 - 4 + dec);
            break;
        case 2:
            lcd.gotoend();
            if (*val > l_limit + 0.1)
                *val = *val - 0.1;
            lcd.printfBackward("%s%05.2f", (*val >= 0 && *val < 100) ? " " : "", *val);
            lcd.gotoX(15 - 3 + dec);
            break;
        case 3:
            lcd.gotoend();
            if (*val > l_limit + 0.01)
                *val = *val - 0.01;
            lcd.printfBackward("%s%05.2f", (*val >= 0 && *val < 100) ? " " : "", *val);
            lcd.gotoX(15 - 3 + dec);
            break;
        default:
            break;
        }
}
void LCD_UI::change_handle_float(float val)
{
    pre_val = val;
    if (on_change_threshold)
    {
        change_p++;
        if (change_p == 4)
        {
            lcd.cursor(setting->ui.cursor);
            // saveOutputSetting(setting);
            on_change_threshold = change_p = 0;
            _n_page = RELOAD_DUMY;
        }
        else
        {
            if (change_p < 2)
                lcd.gotoX(15 - 4 + change_p);
            else
                lcd.gotoX(15 - 3 + change_p);
        }
    }
    else
    {
        lcd.cursor(true);
        lcd.gotoX(15 - 4 + change_p);
        on_change_threshold = 1;
        change_p = 0;
    }
}
void LCD_UI::change_value(bool incre, uint8_t *val, uint8_t h_limit, uint8_t l_limit)
{
    if (incre)
    {
        lcd.gotoend();
        if (*val < h_limit)
            *val = *val + 1;
        lcd.printfBackward("%02u", *val);
        lcd.gotoend();
    }
    else
    {
        lcd.gotoend();
        if (*val > l_limit)
            *val = *val - 1;
        lcd.printfBackward("%.2u", *val);
        lcd.gotoend();
    }
}
void LCD_UI::change_handle_u8(uint8_t *val, uint8_t num)
{
    // Change value of the array val at number num
    val += num;
    if (on_change)
    {
        saveFactorSetting(setting);
        lcd.cursor(setting->ui.cursor);
        on_change = 0;
        _n_page = RELOAD_DUMY;
    }
    else
    {
        pre_val_u8 = *val;
        lcd.cursor(true);
        lcd.gotoX(15);
        on_change = 1;
    }
}

void LCD_UI::yesno_logic()
{
    if (BACK_S)
    {
        _page = _page_ys;
    }
    else if (SELECT_S)
    {
        if (c_yn == c_yes)
        {
        }
        else
        {
        }
    }
    else if (DOWN_S)
    {
        yesno();
    }
    else if (UP_S)
    {
        yesno();
    }
}

//Refesh

void LCD_UI::dashboard_refesh(sht_31_t sht)
{
    lcd.gotoXY(15, 1);
    lcd.printfBackward(" %.2f", sht.temp);
    lcd.gotoXY(15, 2);
    lcd.printfBackward(" %.2f", sht.humi);
    lcd.gotoXY(15, 0);
    pre_sht.temp = sht.temp;
    pre_sht.humi = sht.humi;
}

//Private

void LCD_UI::optionOF(const char *buf, bool val, bool available)
{
    lcd.clearRow();
    lcd.write('>');
    lcd.printfdata(buf);
    lcd.gotoend();
    lcd.printfBackward("%s", (val ? "ON" : "OFF"));
    lcd.nextline(true);
}
void LCD_UI::option(const char *buf, float val, bool available)
{
    lcd.clearRow();
    if (available)
        lcd.write('>');
    else
        lcd.write(0xa5);
    lcd.printfdata(buf);
    lcd.gotoend();
    lcd.printfBackward("%.2f", val);
    lcd.nextline(true);
}
void LCD_UI::option(const char *buf, uint8_t val, bool available)
{
    lcd.clearRow();
    if (available)
        lcd.write('>');
    else
        lcd.write(0xa5);
    lcd.printfdata(buf);
    lcd.gotoend();
    lcd.printfBackward("%02u", val);
    lcd.nextline(true);
}
void LCD_UI::option(const char *buf, bool available)
{
    lcd.clearRow();
    if (available)
        lcd.write('>');
    else
        lcd.write(0xa5);
    lcd.printfdata(buf);
    lcd.gotobegin();
    lcd.nextline(true);
}
void LCD_UI::optionCustom(const char *buf, const char *ifTrue, const char *ifFalse, bool state)
{
    lcd.clearRow();
    lcd.write('>');
    lcd.printfdata(buf);
    lcd.gotoend();
    if (state)
        lcd.printfBackward(ifTrue);
    else
        lcd.printfBackward(ifFalse);
    lcd.nextline(true);
}
void LCD_UI::optionCustom(const char *buf, JSONVar custom, uint8_t state)
{
    lcd.clearRow();
    lcd.write('>');
    lcd.printfdata(buf);
    lcd.gotoend();
    lcd.printfBackward(custom[state]);
    lcd.nextline(true);
}
void LCD_UI::optionYN(const char *yes, const char *no, bool priority)
{
    lcd.clearRow();
    lcd.gotobegin();
    lcd.printfdata(">%s", yes);
    lcd.gotoend();
    lcd.printfBackward(">%s", no);
    c_no = lcd.getcursor() + 1;
    if (priority)
    {
        lcd.gotobegin();
        lcd.writenotmove(0);
        c_yn = c_yes = 0;
    }

    else
    {
        lcd.movecursor(1);
        lcd.writenotmove(0);
        c_yn = c_no;
    }
}

void LCD_UI::split(const char *col1, const char *col2, uint8_t st1, int st2)
{
    st1 = st1 > 7 ? 7 : st1;
    st2 = st2 > 7 ? 7 : st2;
    lcd.clearRow();
    lcd.gotoX(7 - st1);
    lcd.printfBackward(col1);
    // lcd.gotoX(8);
    // lcd.write('|');
    lcd.gotoX(st2 + 9);
    lcd.printfdata(col2);
    lcd.nextline(true);
}
void LCD_UI::split(uint8_t num, sht_31_t sht)
{
    if (num == 0)
    {
        lcd.gotoX(3);
    }
    else
    {
        lcd.clearRow();
        lcd.printfdata("%u", num);
        lcd.write('.');
        lcd.movecursor(1);
    }
    lcd.printfdata("%.2f", sht.temp);
    lcd.gotoX(8);
    // lcd.write('|');
    lcd.movecursor(1);
    lcd.printfdata("%.2f", sht.humi);
    lcd.nextline(true);
}

void LCD_UI::scroll(void *func, uint8_t step, bool back_top)
{
}
void LCD_UI::scrollable(bool up, bool dw)
{
    lcd.gotoXY(14, 0);
    if (up && dw)
        lcd.writenotmove(LCD_CUSTOM_UP_DOWN);
    else if (up)

        lcd.writenotmove(LCD_CUSTOM_UP);

    else if (dw)
        lcd.writenotmove(LCD_CUSTOM_DOWN);
    else
        lcd.writenotmove(' ');
}
void LCD_UI::optional_view1(void)
{
    lcd.gotoXY(0, 2);
    lcd.clearRow();
    lcd.gotoXY(0, 3);
    lcd.clearRow();
    lcd.gotoXY(0, 1);
    lcd.writenotmove(0);
}
void LCD_UI::optional_view2(uint8_t opt)
{
    switch (opt)
    {
    case 0:
        lcd.gotoXY(0, 2);
        lcd.write('>');
        lcd.gotoXY(0, 1);
        lcd.writenotmove(0);
        break;
    case 1:
        lcd.gotoXY(0, 1);
        lcd.writenotmove('>');
        lcd.gotoXY(0, 2);
        lcd.writenotmove(0);
        break;
    default:
        // lcd.gotoXY(0, 1);
        // lcd.writenotmove('>');
        // lcd.gotoXY(0, 2);
        // lcd.writenotmove('>');
        // lcd.gotoXY(0, 3);
        // lcd.writenotmove(0);
        break;
    }
}
void LCD_UI::optional_view(uint8_t opt)
{
    opt = opt > 2 ? opt % 3 : opt;
    switch (opt)
    {
    case 0:
        lcd.gotoXY(0, 3);
        lcd.write('>');
        lcd.gotoXY(0, 2);
        lcd.write('>');
        lcd.gotoXY(0, 1);
        lcd.writenotmove(0);
        break;
    case 1:
        lcd.gotoXY(0, 1);
        lcd.writenotmove('>');
        lcd.gotoXY(0, 3);
        lcd.writenotmove('>');
        lcd.gotoXY(0, 2);
        lcd.writenotmove(0);
        break;
    case 2:
        lcd.gotoXY(0, 1);
        lcd.write('>');
        lcd.nextline(true);
        lcd.write('>');
        lcd.gotoXY(0, 3);
        lcd.writenotmove(0);
        break;
    default:
        // lcd.gotoXY(0, 3);
        // lcd.writenotmove(0);
        break;
        break;
    }
}
void LCD_UI::optionOF_change(bool val)
{
    lcd.gotoend();
    lcd.printfBackward("%s", (val ? " ON" : "OFF"));
    lcd.gotobegin();
}
void LCD_UI::clear_bt()
{
    state_bt[0] = 0;
    state_bt[1] = 0;
    state_bt[2] = 0;
    state_bt[3] = 0;
}

void LCD_UI::saveFactorSetting(ui_setting_t *st)
{
    Serial.println("[st_input_factor_page][saveFactorSetting]Save to EEPROM\r\n");
    // Serial.println("");
    EEPROM.put(EEPROM_SETTING, *(st->input));
    EEPROM.commit();
}
void LCD_UI::readFactorSetting(ui_setting_t *st)
{
    Serial.println("[st_input_factor_page][readFactorSetting]Read from EEPROM\r\n");
    EEPROM.get(EEPROM_SETTING, *(st->input));
    // Serial.println("");
}

void LCD_UI::saveOutputSetting(ui_setting_t *st)
{
    // EEPROM.put(EEPROM_SETTING + 16, _st);
    // EEPROM.commit();
    if (myEEPROM.isConnected())
        myEEPROM.put(MY_EEPROM_OUTPUT_ADDRESS, *st->output);
    Serial.printf("[saveOutputSetting] Relay 1: %5.02f - %.02f\r\n", st->output->relay_1.threshold.temp_h, st->output->relay_1.current);
    Serial.printf("[saveOutputSetting] Relay 2: %5.02f\r\n", st->output->relay_2.threshold.temp_h);
}
void LCD_UI::readOutputSetting(ui_setting_t *st)
{
    // st_output_t _st;

    // EEPROM.get(EEPROM_SETTING + 16, _st);

    if (myEEPROM.isConnected())
        myEEPROM.get(MY_EEPROM_OUTPUT_ADDRESS, *st->output);
    // *st->output = _st;
    Serial.printf("[readOutputSetting] Relay 1: %5.02f - %.02f\r\n", st->output->relay_1.threshold.temp_h, st->output->relay_1.current);
    Serial.printf("[readOutputSetting] Relay 2: %5.02f\r\n", st->output->relay_2.threshold.temp_h);
}