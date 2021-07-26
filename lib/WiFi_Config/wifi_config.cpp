#include "wifi_config.h"

IPAddress ip_ap(192, 168, 1, 1);
IPAddress gateway_ap(192, 168, 1, 1);
IPAddress subnet_ap(255, 255, 255, 0);

WifiConfigClass::WifiConfigClass(void)
{
    _ap_ssid = "Renan Wifi";
    _ap_pass = "iotvision@2021";
}

void WifiConfigClass::begin(String *_ssid, String *_pass)
{
    ssid = _ssid;
    pass = _pass;
    Serial.println("WiFi begin");
    if (readEEPROM())
    {
        WiFi.mode(WIFI_AP_STA);
        connect(*ssid, *pass);
        Serial.printf("[wifi_begin]%s|%s\r\n", (*ssid).c_str(), (*pass).c_str());
        softAP();
    }
    else
    {
        softAP();
        Serial.println("Nothing in EEPROM");
    }
}
void WifiConfigClass::begin(String ap_ssid, String ap_pass)
{
    _ap_ssid = ap_ssid;
    _ap_pass = ap_pass;
    if (readEEPROM())
    {
        WiFi.mode(WIFI_AP_STA);
        connect(*ssid, *pass);
    }
    else
    {
        softAP();
        Serial.println("Nothing in EEPROM");
    }
}

bool WifiConfigClass::readEEPROM()
{
    if (EEPROM.read(EEPROM_SSID) != 0)
    {
        *ssid = "";
        *pass = "";
        for (uint16_t i = EEPROM_SSID; i < EEPROM_PASS; ++i)
        {
            *ssid += char(EEPROM.read(i));
        }
        for (uint16_t i = EEPROM_PASS; i < EEPROM_MAX; ++i)
        {
            *pass += char(EEPROM.read(i));
        }
        *ssid = (*ssid).c_str();
        *pass = (*pass).c_str();
        Serial.printf("[readEEPROM] %s | %s \r\n", (*ssid).c_str(), (*pass).c_str());
        return true;
    }
    return false;
}

void WifiConfigClass::connect(String ssid, String pass)
{
    WiFi.begin(ssid, pass);
}

bool WifiConfigClass::softAP()
{
    WiFi.softAPConfig(ip_ap, gateway_ap, subnet_ap);
    return WiFi.softAP(_ap_ssid, _ap_pass);
}

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_WIFI_CONFIG)
WifiConfigClass WifiConfig;
#endif