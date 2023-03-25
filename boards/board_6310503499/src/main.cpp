// import library
#include <Arduino.h>
#include <Servo.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "config.h"

Servo myservo;

WiFiClient client;
PubSubClient mqtt(client);

int time_interval;

// HTTP
const String baseUrl = "https://foodfeeder.tawanchai.com/";

int get_time_interval()
{
  DynamicJsonDocument doc(2048);
  const String url = baseUrl + "parameter";
  HTTPClient http;
  http.begin(url);
  int httpResponseCode = http.GET();

  if (httpResponseCode >= 200 && httpResponseCode < 300)
  {
    Serial.print("HTTP ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    deserializeJson(doc, payload);
    return (int)doc["timeinterval"];
  }
  else
  {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.println(topic);
  payload[length] = '\0';
  String topic_str = topic, payload_str = (char *)payload;
  Serial.println("[" + topic_str + "]: " + payload_str);
  if (topic_str == "b6310500287/send_feed_time")
  {
    int feed_time = atoi((char *)payload) * 1000;
    Serial.println(time_interval);
    myservo.write(90);
    delay(500);
    myservo.write(0);
    delay(feed_time);
    myservo.write(90);
    delay(500);
    myservo.write(180);
    delay(time_interval);
  }
  if (topic_str == "b6310500287/time_interval")
  {
    time_interval = atoi((char *)payload) * 1000;
    Serial.println(time_interval);
  }
}

// Connect Wifi
void connectToWiFi()
{
  Serial.println("Connected...");

  WiFi.begin(WIFI_NAME, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    digitalWrite(LED, 0);
    delay(1000);
  }
  digitalWrite(LED, 1);
}

void setup()
{
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, 0);
  myservo.attach(SERVO);
  myservo.write(180);
  connectToWiFi();
  mqtt.setServer(MQTT_BROKER, MQTT_PORT);
  mqtt.setCallback(callback);
  time_interval = get_time_interval() * 1000;
}

void loop()
{
  if (!mqtt.connected())
  {
    Serial.print("MQTT connection... ");

    while (!mqtt.connected())
    {
      Serial.println("Reconnecting to MQTT Broker..");

      if (mqtt.connect(MQTT_NAME, MQTT_USERNAME, MQTT_PASSWORD))
      {
        Serial.println("Connected.... Success");
        mqtt.subscribe("b6310500287/send_feed_time");
        mqtt.subscribe("b6310500287/time_interval");
      }
    }
  }
  else
  {
    mqtt.loop();
  }
}
