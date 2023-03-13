#include <Arduino.h>
#include <Wire.h>
#include <PubSubClient.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <menu.h>
#include <menuIO/serialOut.h>
#include <menuIO/serialIn.h>
#include <menuIO/altKeyIn.h>
#include <menuIO/chainStream.h>
#include <menuIO/u8g2Out.h>
#include <U8g2lib.h>
#include "config.h"

WiFiClient wifiClient;
PubSubClient mqtt(MQTT_BROKER, 1883, wifiClient);
const String baseUrl = "https://foodfeeder.tawanchai.com";

using namespace Menu;

#define LEDPIN 2
#define SOFT_DEBOUNCE_MS 100
#define MAX_DEPTH 2

// pins for buttons:
#define BTN_SEL 23  // Select button
#define BTN_UP 25   // Up
#define BTN_DOWN 26 // Down

#define fontName u8g2_font_7x13_mf
#define fontX 7
#define fontY 16
#define offsetX 0
#define offsetY 3
#define U8_Width 128
#define U8_Height 64
#define USE_HWI2C

U8G2_SSD1306_128X64_NONAME_1_HW_I2C display(U8G2_R0, U8X8_PIN_NONE, 22, 21);

U8X8_SSD1306_128X64_NONAME_HW_I2C display_connect(U8X8_PIN_NONE);

char buf[10]; 

// U8X8_SSD1306_128X64_NONAME_HW_I2C display(U8X8_PIN_NONE);

// define menu colors --------------------------------------------------------
// each color is in the format:
//  {{disabled normal,disabled selected},{enabled normal,enabled selected, enabled editing}}
// this is a monochromatic color table
const colorDef<uint8_t> colors[6] MEMMODE = {
    {{0, 0}, {0, 1, 1}}, // bgColor
    {{1, 1}, {1, 0, 0}}, // fgColor
    {{1, 1}, {1, 0, 0}}, // valColor
    {{1, 1}, {1, 0, 0}}, // unitColor
    {{0, 1}, {0, 0, 1}}, // cursorColor
    {{1, 1}, {1, 0, 0}}, // titleColor
};

char *constMEM hexDigit MEMMODE = "0123456789ABCDEF";
char *constMEM hexNr[] MEMMODE = {"0", "x", hexDigit, hexDigit};
char buf1[] = "0x11"; //<-- menu will edit this text

int chooseTest = 2;
int listColor = 1;
int timeinterval = 10;
int feedtime = 10;
int ledCtrl = LOW;


void mqtt_callback(char *topic, byte *payload, unsigned int length);

void GET_parameter(){
  display_connect.drawString(0,0,"Getting");
  display_connect.draw2x2String(0,3,"Param ");
  DynamicJsonDocument doc(2048);
  const String url = baseUrl + "/parameter";
  HTTPClient http;
  // http.setReuse(false);
  // http.setTimeout(10000);
  http.begin(url);
  int httpResponseCode = http.GET();

  if (httpResponseCode >= 200 && httpResponseCode < 300) {
      display_connect.drawString(0,6,"Status: ");
      snprintf(buf, 10, "%2d", httpResponseCode);
      display_connect.drawString(8,6,buf);
      Serial.print("HTTP ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      deserializeJson(doc,payload);
      listColor = (int)doc["color"];
      timeinterval = (int)doc["timeinterval"]; 
      feedtime = (int)doc["feedtime"]; 
  }
  else {
      display_connect.drawString(0,6,"Error: ");
      snprintf(buf, 10, "%2d", httpResponseCode);
      display_connect.drawString(8,6,buf);
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
  }
}

void connect_wifi()
{
  printf("WiFi MAC address is %s\n", WiFi.macAddress().c_str());
  printf("Connecting to WiFi %s.\n", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  // WiFi.addAP(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    display_connect.drawString(0,0,"Connecting");
    display_connect.draw2x2String(0,3,"Wifi ");
    printf(".");
    fflush(stdout);
    delay(500);
  }
  printf("\nWiFi connected.\n");
  // GET_parameter();
}

void connect_mqtt()
{
  printf("Connecting to MQTT broker at %s.\n", MQTT_BROKER);
  if (!mqtt.connect("", MQTT_USER, MQTT_PASS))
  {
    printf("Failed to connect to MQTT broker.\n");
    for (;;)
    {
    } // wait here forever
  }
  mqtt.setCallback(mqtt_callback);
  printf("MQTT broker connected.\n");
}

void mqtt_callback(char *topic, byte *payload, unsigned int length)
{
  if (strcmp(topic, "b6310500287/test") == 0)
  {
    payload[length] = 0;
    int value = atoi((char *)payload);
  }
}

result myLedOn()
{
  ledCtrl = 1;
  String payload(ledCtrl);
  mqtt.publish("b6310500287/light", payload.c_str());
  return proceed;
}
result myLedOff()
{
  ledCtrl = 0;
  String payload(ledCtrl);
  mqtt.publish("b6310500287/light", payload.c_str());
  return proceed;
}

result setColor()
{
  String payload(listColor);
  mqtt.publish("b6310500287/color", payload.c_str());
  return proceed;
}

result setInterval()
{
  String payload(timeinterval);
  mqtt.publish("b6310500287/time_interval", payload.c_str());
  return proceed;
}

result setFeed()
{
  String payload(feedtime);
  mqtt.publish("b6310500287/feed_time", payload.c_str());
  return proceed;
}

CHOOSE(listColor, chooseColor, "Color: ", doNothing, noEvent, noStyle, 
  VALUE("Red", 1, setColor, enterEvent), 
  VALUE("Green", 2, setColor, enterEvent), 
  VALUE("Blue", 3, setColor, enterEvent));

CHOOSE(timeinterval, choosetinterval, "T_interval: ", doNothing, noEvent, noStyle, 
  VALUE("3 s", 3, setInterval, enterEvent), 
  VALUE("5 s", 5, setInterval, enterEvent), 
  VALUE("10 s", 10, setInterval, enterEvent));

CHOOSE(feedtime, choosefeedtime, "T_feed: ", doNothing, noEvent, noStyle, 
  VALUE("3 s", 3, setFeed, enterEvent), 
  VALUE("5 s", 5, setFeed, enterEvent), 
  VALUE("10 s", 10, setFeed, enterEvent));

// CHOOSE(chooseTest, chooseMenu, "Choose", doNothing, noEvent, noStyle, 
//   VALUE("First", 1, doNothing, noEvent), 
//   VALUE("Second", 2, doNothing, noEvent), 
//   VALUE("Third", 3, doNothing, noEvent),
//   VALUE("Last", -1, doNothing, noEvent));

// TOGGLE(ledCtrl,setLed,"Led: ",doNothing,noEvent,wrapStyle,//,doExit,enterEvent,noStyle
//   VALUE("On", HIGH, myLedOn, enterEvent), 
//   VALUE("Off", LOW, myLedOff, enterEvent));

MENU(mainMenu, "     Main Menu", doNothing, noEvent, wrapStyle
  ,SUBMENU(chooseColor),SUBMENU(choosetinterval),SUBMENU(choosefeedtime)
  ,EXIT("<Close Screen>"));

MENU_OUTPUTS(out, MAX_DEPTH, NONE/*SERIAL_OUT(Serial)*/, U8G2_OUT(display, colors, fontX, fontY, offsetX, offsetY, {0, 0, U8_Width / fontX, U8_Height / fontY}) /* x_offset, y_offset, x_width, y_width*/
);

// define button list and functions:
keyMap btn_map[] = {
    {BTN_SEL, defaultNavCodes[enterCmd].ch, INPUT_PULLUP},
    {BTN_UP, defaultNavCodes[upCmd].ch, INPUT_PULLUP},
    {BTN_DOWN, defaultNavCodes[downCmd].ch, INPUT_PULLUP}};
keyIn<3> btns(btn_map); // 3 buttons

// serialIn serial(Serial);
menuIn *inputsList[] = {&btns};
chainStream<1> in(inputsList); // 2 is the number of inputs

NAVROOT(nav, mainMenu, MAX_DEPTH, in, out);

void setup()
{
  pinMode(LEDPIN, OUTPUT);
  Serial.begin(115200);
  while (!Serial)
    ;
  btns.begin();
  Serial.println("Menu test");
  display_connect.begin();
  display_connect.setPowerSave(0);
  display_connect.setFont(u8x8_font_pxplusibmcgathin_f);
  display_connect.drawString(0,0,"Hello Setup");
  display.begin();
  display.setFont(fontName);
  connect_wifi();
  GET_parameter();
  connect_mqtt();
  delay(500);
  delay(1000);
}

void loop()
{
  nav.doInput();
  if (nav.changed(0))
  { // only draw if menu changed for gfx device
    // change checking leaves more time for other tasks
    display.firstPage();
    do
      nav.doOutput();
    while (display.nextPage());
  }
  digitalWrite(LEDPIN, ledCtrl);
  delay(100);
  if (!mqtt.connected())
  {
    while (!mqtt.connected())
    {
      connect_mqtt();
    }
  }
}
