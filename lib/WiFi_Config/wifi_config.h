#pragma once
#include <Arduino.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ArduinoOTA.h>
#include "Arduino_JSON.h"
#include "htmp_page.h"

#define EEPROM_SSID (1024 - 65 - 65)
#define EEPROM_PASS (1024 - 65)
#define EEPROM_MAX (1023)

class WifiConfigClass
{
public:
    WifiConfigClass(void);

    void begin(String *_ssid, String *_pass);
    void begin(String &ap_ssid, String &ap_pass);
    void connect(String ssid, String pass);
    String *ssid, *pass;

private:
    bool readEEPROM();
    bool softAP();
    String _ap_ssid, _ap_pass;

protected:
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_WIFI_CONFIG)
extern WifiConfigClass WifiConfig;
#endif