#include <Arduino.h>
#include "RTClib.h"
#include "SHT31.h"
#include "LCD.h"
#include "time_keeper.h"
#include "lcd_ui.h"
#include "main.h"
#include "wifi_config.h"
#include "myEEPROM.h"
#include "output_driver.h"
#include <ESP8266HTTPClient.h>
#include "FirebaseESP8266.h"

HTTPClient http;
String URL_TIME_API = "http://worldtimeapi.org/api/timezone/Asia/Ho_Chi_Minh";

#define FIREBASE_HOST "cordycepsdata-2bf3a-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "zErCXvWTYYYHdoNN7ZDzsZAigXhJpLsDRN1K5piw"
// #define FIREBASE_AUTH ""

#define _DEBUG

#define LED 2
#define INTP_PIN 12
#define K1_PIN 4
#define K2_PIN 5
#define SHT31_ADDRESS 0x44
#define PCA9535_ADDRESS 0x20
ESP8266WebServer server(80);
bool button[4];
bit_typedef *button_b = (bit_typedef *)&button;
PCA9535 myExpan;
RTC_DS1307 rtc;
SHT31 sht;
QWIICMUX myMux;
LCD lcd;
LCD_UI ui;
// TIME_KEEPER tk;
static String ssid, pass;
static st_input_t factor_st = {
    .t_factor = {1, 1, 1, 1, 1, 1, 1, 1},
    .h_factor = {1, 1, 1, 1, 1, 1, 1, 1},
};
typedef struct
{
  uint8_t num1;
  uint8_t num2;
  uint8_t array1[10];
} test_t;

test_t myStruct = {
    .num1 = 0x25,
    .num2 = 0xab,
    .array1 = {0x01, 0x02, 0x03, 0x04, 0x05},
};

typedef struct
{
  test_t *p;
} p_test_t;

p_test_t test_p = {
    .p = &myStruct,
};
static st_output_t output_st;
static st_threshold_t threshold_st;
ui_setting_t ui_setting = {
    .threshold = &threshold_st,
    .input = &factor_st,
    .output = &output_st,
};
sht_31_t sht_full[8] = {
    {.temp = 11.11, .humi = 11.11},
    {.temp = 22.22, .humi = 22.22},
    {.temp = 33.33, .humi = 33.33},
    {.temp = 44, .humi = 44},
    {.temp = 55.55, .humi = 55},
    {.temp = 66, .humi = 66.66},
    {.temp = 77, .humi = 77},
    {.temp = 8.8, .humi = 8.8},
};
sht_31_t sht_avg, pre_sht_avg;
bool st;
uint8_t c = 0x0f;
bool output_state[5] = {LOW, LOW, LOW, LOW, LOW};
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
byte currentPortNumber;
bool inpt_flag = false;
void ScanForAddress();
void GetTempAndHumi();
//Bien tam hay xoa
uint8_t tam;
uint8_t a = 0;
void h(uint8_t &val)
{
  uint8_t b = val;
  b++;
  Serial.printf("\r\nb: %u %u", b, a);
  uint8_t *c = &val;
  *c = *c + 1;
  Serial.printf("\r\nc: %u %u", *c, a);
  val++;
  Serial.printf("\r\nVal: %u %u\r\n", val, a);
}

// FirebaseConfig FBConfig;
// FirebaseAuth FBAuth;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  h(a);
  Wire.begin();
  Wire.setClock(100000);

  // rtc begin
  if (!rtc.begin())
    Serial.println("Couldn't find RTC!");
  else
  {
    Serial.println("Found RTC!");
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  if (!rtc.isrunning())
  {
    Serial.println("RTC is not running!");
  }
  else
  {
    Serial.println("RTC is running!");
    // rtc.writenvram(0U, 0xab);
    Serial.printf("VRAM: 0x%02x \r\n", rtc.readnvram(0U));
  }
  if (myEEPROM.isConnected())
  {
    Serial.println("EEPROM conected!!");
  }
  else
  {
    Serial.println("Not connect");
  }
  EEPROM.begin(EEPROM_MAX);
  WiFi.disconnect();
  // WifiConfig.begin(&ssid, &pass);
  ssid = "Thao TPLINK";
  pass = "05122601";
  WifiConfig.begin(ssid, pass);
  Serial.printf("Connecting.");
  uint16_t timeout = 0;

  while (!WiFi.isConnected())
  {
    timeout++;
    if (timeout > 10)
    {
      Serial.print(" Fail to conect!");
      break;
    }
    printf(".");
    delay(1000);
  }

  Serial.println();
  server.on("/", mainPage_callback);
  server.on("/test", testPage);
  server.on("/scanWifi", scanWifi);
  server.on("/writeEEPROM", write_EEPROM);
  server.begin();
  pinMode(LED, OUTPUT);

  // i2c mux begin
  if (myMux.begin() == false)
  {
    Serial.println("Mux not detected.");
  }
  else
  {
    Serial.println("Mux detected");
    myMux.setPort(1); //Connect master to port labeled '1' on the mux
    Serial.print("CurrentPort: ");
    Serial.println(currentPortNumber);
  }

  // sht31 begin
  sht.begin(SHT31_ADDRESS, SDA, SCL);

  // pca9535 begin
  myExpan.begin(PCA9535_ADDRESS);
  myExpan.portMode(1, 0x0f);
  myExpan.digitalWrite(1, K1_PIN, output_state[0]);
  myExpan.digitalWrite(1, K2_PIN, output_state[1]);
  for (uint8_t i = 0; i < 8; i++)
  {
    factor_st.t_factor[i] = 2;
    factor_st.h_factor[i] = 2;
  }
  ui.begin(OutputDriver.button, sht_full, &ui_setting);
  rtc.now().unixtime();
  rtc.now().unixtime();
  OutputDriver.init(&myExpan, &ui);
  ui.updateRTC((time_t)rtc.now().unixtime());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}
bool GTECompare(float val, float set)
{
  if (val >= set)
    return HIGH;
  return LOW;
}
bool LTECompare(float val, float set)
{
  if (val <= set)
    return HIGH;
  return LOW;
}

void outPutThresholdDrive(sht_31_t val, st_output_config_t output_set, bool *pre_state, uint8_t output_pin, PCA9535 &Expan)
{
  bool state = *pre_state;
  switch (output_set.type)
  {
  case 0:
    if (output_set.isHeater)
    {
      if (LTECompare(val.temp, output_set.threshold.temp_l))
        state = HIGH;
      else if (GTECompare(val.temp, output_set.threshold.temp_h))
        state = LOW;
    }
    else
    {
      if (LTECompare(val.temp, output_set.threshold.temp_l))
        state = LOW;
      else if (GTECompare(val.temp, output_set.threshold.temp_h))
        state = HIGH;
    }
    break;
  case 1:
    if (output_set.isHeater)
    {
      if (LTECompare(val.temp, output_set.threshold.humi_l))
        state = HIGH;
      else if (GTECompare(val.temp, output_set.threshold.humi_h))
        state = LOW;
    }
    else
    {
      if (LTECompare(val.temp, output_set.threshold.humi_l))
        state = LOW;
      else if (GTECompare(val.temp, output_set.threshold.humi_h))
        state = HIGH;
    }
    break;
  case 2:
    /* code */
    break;
  case 3:
    /* code */
    break;
  }
  if (state != *pre_state)
  {
    *pre_state = state;
    Expan.digitalWrite(1, output_pin, state);
  }
}
JSONVar getRTCJson(void)
{
  String string_get;
  JSONVar json_get;
  http.begin(URL_TIME_API);                           //Specify request destination
  http.addHeader("Content-Type", "application/json"); //Specify content-type header
  int httpCodeGet = http.GET();                       //Recever the request
  // Serial.println("Getting......");
  if (httpCodeGet >= 200 && httpCodeGet < 250)
  {
    string_get = http.getString();
    json_get = JSON.parse(string_get);

    time_t unixtime = (time_t)(json_get["unixtime"]) + 25200;
    //Add 60*60*7 second
    rtc.adjust(DateTime((uint32_t)unixtime));
    if (OutputDriver.toTime(unixtime).mday() != ui.rtc_tm.tm_mday)
    {
      Serial.printf("\r\n API update RTC \r\n");
      ui.updateRTC(unixtime);
    }
    // Serial.printf("\r\n DS1307 update RTC \r\n");
    // ui.updateRTC((time_t)rtc.now().unixtime());
    // Serial.printf("Online Unix String: %s\r\n", JSON.stringify(json_get["unixtime"]).c_str());
    // Serial.printf("Online Unix time_t: %lu \r\n", (time_t)(json_get["unixtime"]));
    // Serial.printf("Online Unix time_t: %lu \r\n", unixtime);
    // Serial.print("JSON String: ");
    // Serial.println(string_get);

    // Serial.print("JSONVar: ");
    // Serial.println(JSON.stringify(json_get));
    return json_get;
  }
  // Serial.printf("\r\n DS1307 update RTC \r\n");
  ui.updateRTC((time_t)rtc.now().unixtime());
  Serial.printf("Code Error: %i \r\n", httpCodeGet);
  return json_get;
}

FirebaseData firebaseData;

void loop()
{
  server.handleClient();
  tk.time_keeper_begin();
  ui.loop();
  OutputDriver.loop();
  if (st)
  {
    st = 0;
  }
  else
  {
    st = 1;
  }
  if (tk.time_flag.t500ms)
  {
    digitalWrite(LED, !digitalRead(LED));
    // outPutThresholdDrive(sht_avg, ui_setting.output->relay_1, &output_state[0], K1_PIN, myExpan);
    // outPutThresholdDrive(sht_avg, ui_setting.output->relay_2, &output_state[1], K2_PIN, myExpan);
    // myExpan.digitalWrite(1, K1_PIN, LOW);
    // myExpan.digitalWrite(1, K2_PIN, LOW);
  }

  if (tk.time_flag.t1s)
  {
    // OutputDriver.outputThreshold();
    // Serial.println(ui.setting->output->relay_1.threshold.temp_h);
    // Serial.println(ui_setting.output->relay_1.threshold.temp_h);
    if (rtc.isrunning())
    {
      // DateTime rtc_t = rtc.now();
      // ui_setting.output->relay_1.time_state->date_time[0] = rtc_t.unixtime();
      // ui_setting.output->relay_1.time_state->date_time[1] = rtc_t.unixtime();
      // Serial.printf("Time: %i:%i UnixTime: %ul\r\n", rtc_t.hour(), rtc_t.minute(), rtc_t.unixtime());
      // time_t unix = rtc_t.unixtime();
      // localtime_r(&unix, &ptm);
      // printf("The time is: %02d:%02d:%02d\n", ptm.tm_hour,
      //        ptm.tm_min, ptm.tm_sec);
      // unix = mktime(&ptm);
      // printf("Hour: %u UnixTime: %lu\r\n", rtc_t.hour(), unix);
      // localtime_r(&unix, &ptm);
      // printf("The time is: %02d:%02d:%02d\n", ptm.tm_hour,
      //        ptm.tm_min, ptm.tm_sec);
      // Serial.println("=====================================");
      // Serial.printf("Size of rtc_t %u\r\n", sizeof(rtc_t));
    }

    // else
    // {
    //   rtc.begin();
    // }
    // uint8_t ebuf[16];
    // myEEPROM.read(0x20, ebuf, 10);
    // Serial.printf("MyEEPROM read: 0x%02x 0x%02x 0x%02x 0x%02x\r\n", ebuf[0],ebuf[2],ebuf[8],ebuf[9]);

    // if (ui._page == DASH_BOARD)
    // {
    //   tam++;
    //   tam = tam > 7 ? 0 : tam;
    //   ui.dashboard_refesh(sht_full[tam]);
    // }
    GetTempAndHumi();
  }

  if (tk.time_flag.t5s)
  {
    if (Firebase.setFloat(firebaseData, "/SHT31_AVRG/Temperature", ui.sht_avg.temp))
    {
      Serial.println("PASSED");
      Serial.println("PATH: " + firebaseData.dataPath());
      Serial.println("TYPE: " + firebaseData.dataType());
      Serial.println("ETag: " + firebaseData.ETag());
      Serial.println("------------------------------------");
      Serial.println();
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + firebaseData.errorReason());
      Serial.println("------------------------------------");
      Serial.println();
    }

    if (Firebase.setFloat(firebaseData, "/SHT31_AVRG/Humidity", ui.sht_avg.humi))
    {
      Serial.println("PASSED");
      Serial.println("PATH: " + firebaseData.dataPath());
      Serial.println("TYPE: " + firebaseData.dataType());
      Serial.println("ETag: " + firebaseData.ETag());
      Serial.println("------------------------------------");
      Serial.println();
    }
    else
    {
      Serial.println("FAILED");
      Serial.println("REASON: " + firebaseData.errorReason());
      Serial.println("------------------------------------");
      Serial.println();
    }

    if (ui._page == ST_DETAIL_TIME)
    {
      Serial.printf("UI Time: %02i:%02i - %02i/%02i/%02d\r\n", ui.rtc_tm.tm_hour, ui.rtc_tm.tm_min, ui.rtc_tm.tm_mday, ui.rtc_tm.tm_mon + 1, ui.rtc_tm.tm_year + 1900);
    }
    if (myEEPROM.isConnected())
    {
      //   float input_eeprom[120];
      //   for (uint32_t i = 0; i < 120; i++)
      //     input_eeprom[i] = i*1.7f;
      //   float output_eeprom[120];
      //   uint32_t dif = 0;
      //   myEEPROM.put(MY_EEPROM_OUTPUT_ADDRESS, input_eeprom);
      //   delay(100);
      //   myEEPROM.get(MY_EEPROM_OUTPUT_ADDRESS, output_eeprom);
      //   for (uint32_t i = 0; i < 120; i++){
      //     if(input_eeprom[i] != output_eeprom[i]){
      //       dif++;
      //       Serial.printf("%d, ", i);
      //     }
      //   }
      //   if(dif != 0){
      //     Serial.printf("\r\nHien co %d loi \r\n", dif);
      //   }else{
      //     Serial.println("Khong co khac biet!!");
      //   }

      // st_output_config_t relay1, relay2;
      // ui_setting.output->relay_1.threshold.temp_h = ui_setting.output->relay_1.threshold.temp_l = 25.21f;
      // ui_setting.output->relay_2.threshold.temp_h = ui_setting.output->relay_2.threshold.temp_l = 60.12f;
      // ui_setting.output->relay_1.current = ui_setting.output->relay_2.current = 99.99f;
      // ui_setting.output->relay_1.time_state->date_time[1] = ui_setting.output->relay_2.time_state->date_time[1] = 162321412;
      // ui_setting.output->relay_1.threshold = {
      //     32.23f,
      //     43.43f,
      //     22.42f,
      //     21.23f,
      // };
      // Serial.printf("[setup] Relay 1: %5.02f %5.02f\r\n", output_st.relay_1.threshold.temp_h, output_st.relay_1.threshold.temp_l);
      // Serial.printf("[setup] Relay 2: %5.02f %5.02f\r\n", output_st.relay_2.threshold.temp_h, output_st.relay_2.threshold.temp_l);
      // myEEPROM.put(MY_EEPROM_OUTPUT_ADDRESS, output_st.relay_1);
      // delay(1000);
      // myEEPROM.put(MY_EEPROM_OUTPUT_ADDRESS + sizeof(relay1), output_st.relay_2);
      // delay(500);
      // myEEPROM.get(MY_EEPROM_OUTPUT_ADDRESS, relay1);
      // myEEPROM.get(MY_EEPROM_OUTPUT_ADDRESS + sizeof(relay1), relay2);
      // Serial.printf("\r\n[loop 1s] Relay 1: %5.02f %5.02f\r\n", relay1.threshold.temp_h, relay1.threshold.temp_l);
      // Serial.printf("[loop 1s] Relay 2: %5.02f %5.02f\r\n", relay2.threshold.temp_h, relay2.threshold.temp_l);
      // Serial.println("===========================================================");

      // ui_setting.output->relay_1.threshold.temp_h = ui_setting.output->relay_1.threshold.temp_l = 25.21f;
      // ui_setting.output->relay_2.threshold.temp_h = ui_setting.output->relay_2.threshold.temp_l = 60.12f;
      // Serial.printf("[setup] Relay 1: %5.02f %5.02f\r\n", output_st.relay_1.threshold.temp_h, output_st.relay_1.threshold.temp_l);
      // Serial.printf("[setup] Relay 2: %5.02f %5.02f\r\n", output_st.relay_2.threshold.temp_h, output_st.relay_2.threshold.temp_l);
      // myEEPROM.put(MY_EEPROM_OUTPUT_ADDRESS, output_st);
      // delay(500);
      // st_output_t hi;
      // myEEPROM.get(MY_EEPROM_OUTPUT_ADDRESS, hi);
      // Serial.printf("\r\n[loop 1s] Relay 1: %5.02f %5.02f\r\n", hi.relay_1.threshold.temp_h, hi.relay_1.threshold.temp_l);
      // Serial.printf("[loop 1s] Relay 2: %5.02f %5.02f\r\n", hi.relay_2.threshold.temp_h, hi.relay_2.threshold.temp_l);
      // Serial.println("===========================================================");
    }
  }
  if (tk.time_flag.t10s)
  {
    if ((ui._page == WELCOME) || (ui._page == WELCOME2))
    {
      getRTCJson();
    }
    if (rtc.isrunning())
    {
      // Serial.printf("\r\n DS1307 update RTC \r\n");
      ui.updateRTC((time_t)rtc.now().unixtime());
    }
    // Serial.printf("UI RTC: %lu\r\n", ui.rtc);
    // Serial.printf("UI Time: %02i:%02i - %02i/%02i/%02d\r\n", ui.rtc_tm.tm_hour, ui.rtc_tm.tm_min, ui.rtc_tm.tm_mday, ui.rtc_tm.tm_mon + 1, ui.rtc_tm.tm_year + 1900);

    // ui.rtc = rtc.now().unixtime();
    // struct tm set_tm, rtc_tm;
    // // localtime_r(&ui.rtc, &rtc_tm);
    // for (uint8_t i = 0; i < 6; i++)
    // {
    //   //Start
    //   localtime_r(&ui.setting->output->relay_1.time_state[i].date_time[0], &set_tm);
    //   if ((set_tm.tm_hour == rtc_tm.tm_hour) && (set_tm.tm_min == rtc_tm.tm_min))
    //   {
    //     if (ui.setting->output->relay_1.time_state[i].isUsed)
    //     {
    //       if (ui.setting->output->relay_1.time_state[i].state)
    //       {
    //         //Turn On
    //       }
    //       else
    //       {
    //         //Turn Off
    //       }
    //     }
    //   }
    //   //End
    //   localtime_r(&ui.se0tting->output->relay_1.time_state[i].date_time[1], &set_tm);
    //   if ((set_tm.tm_hour == rtc_tm.tm_hour) && (set_tm.tm_min == rtc_tm.tm_min))
    //   {
    //     if (ui.setting->output->relay_1.time_state[i].isUsed)
    //     {
    //       if (ui.setting->output->relay_1.time_state[i].state)

    //       {
    //         //Turn Off
    //       }
    //       else
    //       {
    //         //Turn On
    //       }
    //     }
    //   }
    // }
  }
  tk.time_keeper_end();
}
void ScanForAddress()
{
  Serial.println();
  byte nDevices = 0;
  for (byte address = 1; address < 127; address++)
  {
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address < 0x10)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println();

      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print("Unknown error at address 0x");
      if (address < 0x10)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }

  if (nDevices == 0)
    Serial.println("No I2C devices found");
  else
    Serial.println("Done");
}
void GetTempAndHumi()
{
  for (uint8_t i = 0; i < 8; i++)
  {
    myMux.setPort(i);
    Wire.beginTransmission(SHT31_ADDRESS);
    if (Wire.endTransmission() == 0)
    {
      currentPortNumber = myMux.getPort();
      sht.read();
      sht_full[i].temp = sht.getTemperature();
      sht_full[i].humi = sht.getHumidity();
      // Serial.printf("Port %u: Temp - %.2f Humi - %.2f \r\n", currentPortNumber, sht_full[i].temp, sht_full[i].humi);
    }
    else
    {
      currentPortNumber = myMux.getPort();
      sht_full[i].temp = 0;
      sht_full[i].humi = 0;
      // Serial.printf("Port %u -----------------------\r\n", currentPortNumber);
    }
  }
  // Serial.println("-------------End Loop---------------");
}
void mainPage_callback()
{
  server.send(200, "text/html", (String)FPSTR(MainPage));
}
void testPage()
{
  server.send(200, "text/html", (String)FPSTR(TestPage));
}

void scanWifi()
{
  int n = WiFi.scanNetworks();
#ifdef _DEBUG
  Serial.println("[webapp]Scan Wifi");
#endif
  String s = "<div><b>Danh sách quét được</b></div>";
  for (int i = 0; i < n; i++)
  {
    s += "<p class = \"center_p\" onClick = \"document.getElementById('ssid').value = '";
    s += WiFi.SSID(i);
    s += "'\"><u>";
    s += WiFi.SSID(i);
    s += " ";
    // s+= WiFi.channel(i); s+= " ";
    s += WiFi.RSSI(i) * (-1);
    s += "% ";
    s += WiFi.encryptionType(i) == ENC_TYPE_NONE ? " -> Open" : "";
    s += "</u></p>";
  }
  server.send(200, "text/html", s);
}
void write_EEPROM()
{
  ssid = server.arg("ssid");
  pass = server.arg("pass");
#ifdef _DEBUG
  Serial.println("[write_EEPROM]Clear EEPROM before write!");
#endif
  for (uint16_t i = EEPROM_SSID; i < EEPROM_MAX; ++i)
  {
    EEPROM.write(i, 0);
    delay(10);
  }
#ifdef _DEBUG
  Serial.println("[write_EEPROM]Write EEPROM!");
#endif
  for (uint16_t i = 0; i < ssid.length(); ++i)
  {
    EEPROM.write(EEPROM_SSID + i, ssid[i]);
  }
  for (uint16_t i = 0; i < pass.length(); ++i)
  {
    EEPROM.write(EEPROM_PASS + i, pass[i]);
  }
  EEPROM.commit();
#ifdef _DEBUG
  Serial.println("Writed to EEPROM!");
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("PASS: ");
  Serial.println(pass);
#endif
  String s;
  s += "Đã lưu thông tin WiFi - SSID: ";
  s += ssid;
  s += " - PASS: ";
  s += pass;
  WiFi.begin(ssid, pass);
  server.send(200, "text/html", s);
}
