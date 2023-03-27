#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <string>
#include <iostream>
#include <math.h>

#define MQTT_NAME ""
//define color
#define RED 1
#define GREEN 2
#define BLUE 3
#define PURPLE 4
#define YELLOW 5

//define ultrasonic pin
const int trigPin = 15;
const int echoPin = 2;
long duration;
float distanceCm;
float distanceInch;
int globalColor = 0;
int feedTime;
int tray_food_status = 0;
int last_state = 0;
int state = 0;
int count = 10;
//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

//define color sensor pin
#define S0 25
#define S1 26
#define S2 22
#define S3 23
#define sensorOut 32
int frequency = 0;

int public_state = 0;

char buffer[20];

WiFiClient client;
PubSubClient mqtt(client);

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
      Serial.println((int)doc["color"]);
      Serial.println((int)doc["feedtime"]);
      globalColor = (int)doc["color"];
      feedTime = (int)doc["feedtime"];
  }
  else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
  }
}

void connectToWiFi() {
  Serial.println("Connected...");
 
  // ทำการเชื่อมต่อ wifi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  // วนไปเรื่อยถ้ายังเชื่อมต่อไม่ได้
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  
  // เช็ค IP Address
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void mqtt_callback(char *topic, byte *payload, unsigned int length)
{
  payload[length] = '\0';
  String payload_str = (char *)payload;

  if (strcmp(topic, "b6310500287/tray_food_status") == 0)
  {
    // payload[length] = '\0';
    tray_food_status = atoi((char *)payload);
    Serial.print("Topic: ");
    Serial.println(topic);
    Serial.print("Payload: ");
    Serial.println(payload_str);
  }
  if (strcmp(topic, "b6310500287/color") == 0)
  {
    // payload[length] = '\0';
    Serial.println("Fetch... in callback");
    GET_post();
  }
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
  mqtt.subscribe("b6310500287/tray_food_status");
  mqtt.subscribe("b6310500287/color");
  printf("MQTT broker connected.\n");
}

int chooseColor(int R, int G, int B){
  // if(B > 400 and G > 400 and R > 400){
  //   return 4;
  // }else if(B < 300 and G < 300 and R < 300){
  //   return 4;
  // }
  if(B > R and B > G){
    return 3;
  }else if(R > B and R > G){
    return 1;
  }else if(G > B and G > R){
    return 2;
  }
  return 0;
}

void setup() {
  Serial.begin(115200); // Starts the serial communication

  connectToWiFi();

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);

  // Setting frequency-scaling to 100%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, HIGH);

  mqtt.setServer(MQTT_BROKER, 1883);
  // mqtt.setCallback(allbackc);
  Serial.println("Fetch...");
  GET_post();
}

void loop() {
  if(!mqtt.connected()){
    connect_mqtt();
  }else{
    mqtt.loop();
  }
  std::string str_tray = std::to_string(tray_food_status);
  // Serial.print("Tray: ");
  // Serial.println(str_tray.c_str());
  Serial.print("Count: ");
  Serial.println(count);
  Serial.print("State: ");
  Serial.print(state);
  Serial.print(" lastState: ");
  Serial.println(last_state);
  Serial.print("PublishState: ");
  Serial.println(public_state);
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;
  
  // Convert to inches
  distanceInch = distanceCm * CM_TO_INCH;
  
  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  // Serial.print("Distance (inch): ");
  // Serial.println(distanceInch);
  
  if (distanceCm <= 30){
     // Setting red filtered photodiodes to be read
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
    // Reading the output frequency
    frequency = pulseIn(sensorOut, LOW);
    // Printing the value on the serial monitor
    // int redFreq = map(frequency,30,270,255,0);
    int redFreq = map(frequency,50,100,255,0);
    Serial.print("R= ");     // printing name
    Serial.print(redFreq); // printing RED color frequency
    Serial.print(", ");
    Serial.print(frequency);
    Serial.print(" ");
    int Red = redFreq;
    delay(100);
    // Setting Green filtered photodiodes to be read
    digitalWrite(S2, HIGH);
    digitalWrite(S3, HIGH);
    // Reading the output frequency
    frequency = pulseIn(sensorOut, LOW);
    // Printing the value on the serial monitor
    // int greenFreq = map(frequency,80,350,255,0);
    int greenFreq = map(frequency,50,100,255,0);
    Serial.print("G= ");     // printing name
    Serial.print(greenFreq); // printing RED color frequency
    Serial.print(", ");
    Serial.print(frequency);
    Serial.print(" ");
    int Green = greenFreq;
    delay(100);
    // Setting Blue filtered photodiodes to be read
    digitalWrite(S2, LOW);
    digitalWrite(S3, HIGH);
    // Reading the output frequency
    frequency = pulseIn(sensorOut, LOW);
    // Printing the value on the serial monitor
    // int blueFreq = map(frequency,10,350,255,0);
    int blueFreq = map(frequency,50,100,255,0);
    Serial.print("B= ");     // printing name
    Serial.print(blueFreq); // printing RED color frequency
    Serial.print(", ");
    Serial.println(frequency);
    int Blue = blueFreq;
    delay(100);
    // Serial.println("Fetch...");
    // GET_post();
    // mqtt.loop();
    int color = chooseColor(Red, Green, Blue);
    // if(color == globalColor){
    //   delay(500);
    //   if(color == globalColor){

    //   }
    // }
    if(color == globalColor){
      state = 1;
      if(state == last_state){
        count--;
        Serial.println("--------------->ColorDetect");
        if(count <= 0){
          if(!public_state){
            Serial.println("detect");
            mqtt.publish("b6310500287/status-detect","1");
            public_state = !public_state;
            count = 10;
          }
        }
      }else{
        count = 10;
      }
      last_state = state;
    }else{
      state = 0;
      if(state==last_state){
        // Serial.println("else 1");
        count--;
        // Serial.println("else 2");
        if(count <= 0){
          if(public_state == 1){
            mqtt.publish("b6310500287/status-detect","0");
          }
          public_state = 0;
          count = 10;
        }
      }else{
        count = 10;
      }
      last_state = state;
    }
  }else{
    state = 0;
    if(state==last_state){
      // Serial.println("else 1");
      count--;
      // Serial.println("else 2");
      if(count <= 0){
        if(public_state == 1){
          mqtt.publish("b6310500287/status-detect","0");
        }
        public_state = 0;
        count = 10;
      }
    }else{
      count = 10;
    }
    last_state = state;
  }
  
  
  delay(200);

  
}