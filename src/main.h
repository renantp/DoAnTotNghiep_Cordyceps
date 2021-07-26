#pragma once
#include <Arduino.h>
#include <EEPROM.h>
#include "SparkFun_I2C_Mux_Arduino_Library.h"
#include <time.h>
#include "PCA9535.h"

struct tm ptm;
void scanWifi();
void testPage();
void mainPage_callback();
void write_EEPROM();