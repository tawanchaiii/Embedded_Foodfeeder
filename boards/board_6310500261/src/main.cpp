#include <Arduino.h>
#include <Servo.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <string>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// config 
#define WIFI_SSID    "GalaxyNote10+25bc"
#define WIFI_PASS    "qbil0530"
#define MQTT_BROKER  "mqtt.tawanchai.com"
#define MQTT_PORT    1883
#define MQTT_USER    "champ"  // ไม่ต้องใส่เครื่องหมาย <>
#define MQTT_PASS    "ztwmdmqi"      // ไม่ต้องใส่เครื่องหมาย <>

#define SERVO 12
#define LDR 32
#define LASER 33
#define LIGHT_TH 1400
Servo servo;
WiFiClient client;
// PubSubClient mqtt(client);
PubSubClient mqtt(MQTT_BROKER, 1883, client);
// 0 => อาหารหมด , 1=> อาหารเหลือ
uint32_t last_publish_status;
int feedTime;
const String baseUrl = "https://foodfeeder.tawanchai.com/"; //url for GET

void GET_post(){
  int ans[2];
  DynamicJsonDocument doc(2048);
  const String url = baseUrl + "parameter";
  HTTPClient http;
  http.begin(url);
  int httpResponseCode = http.GET();

  if (httpResponseCode >= 200 && httpResponseCode < 300) {
      Serial.print("HTTP ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      deserializeJson(doc,payload);

      Serial.println();
      // Serial.println((int)doc["color"]);
      Serial.println((int)doc["feedtime"]);
      // globalColor = (int)doc["color"];
      feedTime = (int)doc["feedtime"];
      Serial.println(feedTime);
  }
  else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
  }
}

//connecting WIFI
void connect_wifi() {
  Serial.println("Connecting WIFI");
 
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  
  Serial.println("WIFI connected");
}

// mqtt callback
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.println(topic);
  payload[length] = '\0';
  String topic_str = topic, payload_str = (char *)payload;
  // Serial.println("[" + topic_str + "]: " + payload_str);
  // Serial.println(topic);

  if (strcmp(topic,"b6310500287/status-detect") == 0 ) {
    int detect_status = atoi((char *)payload);
    Serial.println(detect_status);
    if(detect_status){
      servo.write(45);
      delay(500);
      servo.write(0); // rotate Servo 0 degree
      delay(500);
      if (last_publish_status == 0)
      {
        /* code */
        // GET_post();
        Serial.println("Public");
        char bufferFeedTime[20];
        sprintf(bufferFeedTime, "%d", feedTime);
        Serial.printf("%s",bufferFeedTime);
        mqtt.publish("b6310500287/send_feed_time",bufferFeedTime);
      }
    }
    else{
      delay(feedTime*1000);
      servo.write(45);
      delay(500);
      servo.write(90); // rotate Servo back to 90 degree
      delay(500);
    }
  }
  if (strcmp(topic,"b6310500287/feed_time") == 0 ) {
    feedTime = atoi((char *)payload);
  }

}

// mqtt connect

void connect_mqtt() {
  // printf("Connecting to MQTT broker at %s.\n", MQTT_BROKER);
  if (!mqtt.connected()) {
    printf("Failed to connect to MQTT broker.\n");
    while (!mqtt.connected())
    {
      Serial.println("Reconnecting to MQTT Broker..");

      if (mqtt.connect("", MQTT_USER, MQTT_PASS))
      {
        Serial.println("Connected.... Success");
        mqtt.setCallback(callback);
        mqtt.subscribe("b6310500287/status-detect");
        mqtt.subscribe("b6310500287/feed_time");
        printf("MQTT broker connected.\n");
      }
    }
  }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  connect_wifi();
  mqtt.setServer(MQTT_BROKER, MQTT_PORT);
  pinMode(LASER,OUTPUT);
  digitalWrite(LASER,1);
  servo.attach(SERVO);
  servo.write(90);
  last_publish_status = 0;
  GET_post();

}

void loop() {
  // put your main code here, to run repeatedly:
  connect_mqtt();
  mqtt.loop();
  Serial.println(analogRead(LDR));
  if (analogRead(LDR) <= LIGHT_TH){
    // 1=>มี
    if (analogRead(LDR)<=1350 && last_publish_status == 1){
      // ถ้าอาหารหมด และ สถานะที่เคย publish ครั้งก่อนคือมี
      // publish ใหม่ว่าหมด (0)
      std::string newStatus = std::to_string(0);
      mqtt.publish("b6310500287/tray_food_status",newStatus.c_str());
      last_publish_status = 0;
    }
  }
  else {
      if (analogRead(LDR)>=1550 && last_publish_status == 0){
        std::string newStatus = std::to_string(1);
        mqtt.publish("b6310500287/tray_food_status",newStatus.c_str());
        last_publish_status = 1;
    }
  }
}
