#include "output_driver.h"

DriverClass OutputDriver;
UnixToTime::UnixToTime(time_t &tm)
{
    localtime_r(&tm, &_tm);
    _time = &tm;
}
DriverClass::DriverClass()
{
    for (int i = 0; i < 5; i++)
    {
        output[i] = 0;
    }
}
UnixToTime DriverClass::toTime(time_t &unixtime)
{
    return UnixToTime(unixtime);
}
void DriverClass::CheckCurrent(st_output_config_t &output_config)
{
    if (tk.time_flag.t1s)
    {
        float current_ratio = (current - output_config.current) / output_config.current;
        if (current_ratio >= 0.0f)
        {
            //Increase the current total
            current_total += (1 + current_ratio * 10);
            current_total = current_total > 10 ? 10 : current_total;
        }
        else
        {
            //Decrease the current total
            current_total += (-1 + current_ratio * 10);
            current_total = current_total < 0 ? 0 : current_total;
        }
    }
}
int DriverClass::init(PCA9535 *_PCA9535, LCD_UI *_ui)
{
    myExpan = _PCA9535;
    ui = _ui;
    FIRFilter_Init(&fir);
    return myExpan->begin(PCAA9535_DEFAULT_ADDRESS);
}
void DriverClass::UpdateDate(void)
{
    for (int i = 0; i < 6; i++)
    {
        updateTheDay(&ui->setting->output->relay_1.time_state[i].date_time[0]);
        updateTheDay(&ui->setting->output->relay_1.time_state[i].date_time[1]);
        updateTheDay(&ui->setting->output->relay_2.time_state[i].date_time[0]);
        updateTheDay(&ui->setting->output->relay_2.time_state[i].date_time[1]);
        updateTheDay(&ui->setting->output->out0.time_state[i].date_time[0]);
        updateTheDay(&ui->setting->output->out0.time_state[i].date_time[1]);
        updateTheDay(&ui->setting->output->out1.time_state[i].date_time[0]);
        updateTheDay(&ui->setting->output->out1.time_state[i].date_time[1]);
        updateTheDay(&ui->setting->output->out2.time_state[i].date_time[0]);
        updateTheDay(&ui->setting->output->out2.time_state[i].date_time[1]);
        // toTime(ui->setting->output->relay_1.time_state[i].date_time[0]).hour();
        // toTime(ui->setting->output->relay_2.time_state[i].date_time[0]);
        // toTime(ui->setting->output->out0.time_state[i].date_time[0]);
        // toTime(ui->setting->output->out1.time_state[i].date_time[0]);
        // toTime(ui->setting->output->out2.time_state[i].date_time[0]);
    }
}
void DriverClass::loop(void)
{

    adc_value = analogRead(A0);
    adc_voltage = adc_value * 5.0 / 1024;
    fir_voltage = FIRFilter_Update(&fir, adc_voltage);
    if (tk.time_flag.t100ms)
    {
        // Serial.printf("adc_value: %d | adc_voltage %5.02f | fir_voltage: %5.02f\r\n", adc_value, adc_voltage, fir_voltage);
    }

    if (ui->rtc_tm.tm_mday != toTime(ui->setting->output->out2.time_state[5].date_time[1]).mday())
    {
        Serial.println("Do the update");
        UpdateDate();
        ui->saveOutputSetting(ui->setting);
    }

    //Read Button buffer
    for (int i = 0; i < 4; i++)
    {
        button[i] = myExpan->digitalRead(1, i);
    }
    //Do the current messuament
    CheckCurrent(ui->setting->output->relay_1);
    //
    OutPutThresholdDrive(ui->setting->output->relay_1, &output[0], K1_PIN);
    OutPutThresholdDrive(ui->setting->output->relay_2, &output[1], K2_PIN);
}

void DriverClass::outputThreshold(void)
{
    Serial.println(ui->setting->output->relay_1.threshold.temp_h);
}

//Private-----------------------------------------------------------------------------------------------------------
void DriverClass::OutPutThresholdDrive(st_output_config_t &output_config, uint8_t *output_state, uint8_t output_pin)
{
    uint8_t state = *output_state;
    switch (output_config.type)
    {
    //Temperature Type
    case 0:
        //Heater turn on when below Low Threshold and turn off when above High Threshold
        if (output_config.isHeater)
        {
            if (LessThanOrEqual(ui->sht_avg.temp, output_config.threshold.temp_l))
                state = HIGH;
            else if (GreaterThanOrEqual(ui->sht_avg.temp, output_config.threshold.temp_h))
                state = LOW;
        }
        //Cooler turn on when above High Threshold and turn off when below Low Threshold
        else
        {
            if (GreaterThanOrEqual(ui->sht_avg.temp, output_config.threshold.temp_h))
                state = HIGH;
            else if (LessThanOrEqual(ui->sht_avg.temp, output_config.threshold.temp_l))
                state = LOW;
        }
        break;
    //Humitidity Type
    case 1:
        //Steam turn on when below Low Threshold and turn off when above High Threshold
        if (output_config.isHeater)
        {
            if (LessThanOrEqual(ui->sht_avg.humi, output_config.threshold.humi_l))
                state = HIGH;
            else if (GreaterThanOrEqual(ui->sht_avg.humi, output_config.threshold.humi_h))
                state = LOW;
        }
        //Dryer turn on when above High Threshold and turn off when below Low Threshold
        else
        {
            if (GreaterThanOrEqual(ui->sht_avg.humi, output_config.threshold.humi_h))
                state = HIGH;
            else if (LessThanOrEqual(ui->sht_avg.humi, output_config.threshold.humi_l))
                state = LOW;
        }
        break;
        //Time Perodic Type
    case 2:

        //Loop through all time_state
        for (uint8_t i = 0; i < 6; i++)
        {
            // Is the time perodic used?
            if (output_config.time_state[i].isUsed)
            {
                // Serial.printf("\r\nTime preodic %i is used.\r\n", i);
                //Inside Time Preodic
                if ((output_config.time_state[i].date_time[0] <= ui->rtc) && (output_config.time_state[i].date_time[1] > ui->rtc))
                {
                    state = output_config.time_state[i].state == 1 ? HIGH : LOW;
                    // if (state != *output_state)
                    // Serial.println("Inside time preodic");
                    break;
                }
                //End time perodic
                else if (output_config.time_state[i].date_time[1] <= ui->rtc)
                {
                    state = output_config.time_state[i].state == 1 ? LOW : HIGH;
                    // if (state != *output_state)
                    // Serial.println("End of time preodic");
                    //Not comming yet
                }
                else
                {
                    // Serial.println("Not comming time preodic yet");
                    // state = output_config.time_state[i].state == 1 ? LOW : HIGH;
                }
            }
        }
        break;
    //Current Limit and Temperature type
    case 3:
        //Check the current
        if (current_total >= 10)
        {
            current_total = 0;
            state = LOW;
            break;
        }
        //Heater turn on when below Low Threshold and turn off when above High Threshold
        if (output_config.isHeater)
        {
            if (LessThanOrEqual(ui->sht_avg.temp, output_config.threshold.temp_l))
                state = HIGH;
            else if (GreaterThanOrEqual(ui->sht_avg.temp, output_config.threshold.temp_h))
                state = LOW;
        }
        //Cooler turn on when above High Threshold and turn off when below Low Threshold
        else
        {
            if (GreaterThanOrEqual(ui->sht_avg.temp, output_config.threshold.temp_h))
                state = HIGH;
            else if (LessThanOrEqual(ui->sht_avg.temp, output_config.threshold.temp_l))
                state = LOW;
        }
        break;
    default:
        break;
    }
    if (state != *output_state)
    {
        *output_state = state;
        myExpan->digitalWrite(1, output_pin, *output_state);
        Serial.printf("[OutPutThresholdDrive]Output %s", state == HIGH ? "ON" : "OFF");
    }
}