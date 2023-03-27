#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <string>

//assign pin
const int trigPin = 5;
const int echoPin = 18;

//define wifi name and password
#define WIFI_SSID "Brennius"
#define WIFI_PASS "frvq4195"

//define mqtt server 
#define MQTT_BROKER "mqtt.tawanchai.com"
#define MQTT_PORT 1883
#define MQTT_USER "champ"  // ไม่ต้องใส่เครื่องหมาย <>
#define MQTT_PASS "ztwmdmqi"      // ไม่ต้องใส่เครื่องหมาย <>
#define MQTT_NAME ""

WiFiClient client;
PubSubClient mqtt(client);

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701
//define max tray length in cm
#define MAX_LENGTH 33 

long duration;
float distanceCm;
float distanceInch;
//connecting WIFI
void connectToWiFi() {
  Serial.println("Connecting WIFI");
 
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  
  Serial.println("WIFI connected");
  Serial.println("Mac address");
  Serial.println(WiFi.macAddress());

}


void setup() {
  Serial.begin(115200); // Starts the serial communication
  connectToWiFi(); //Connect to WIFI
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  mqtt.setServer(MQTT_BROKER, MQTT_PORT);
}

void loop() {
    //connecting mqtt
    if (!mqtt.connected()) {
    Serial.print("MQTT connection... ");
    
    while (!mqtt.connected()) {
      Serial.println("Reconnecting to MQTT Broker..");
      
      if (mqtt.connect(MQTT_NAME,MQTT_USER,MQTT_PASS)) {
        Serial.println("Connected.... Success");
        mqtt.publish("b6310500287/debug","Hello World");
      }
    }
  } else {
    mqtt.loop();
  }

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
    
  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);

  //calculating food percentage
  int foodPercentageInt = floor(((MAX_LENGTH-distanceCm)/MAX_LENGTH)*100);
  std::string foodPercentageStr = std::to_string(foodPercentageInt);
  Serial.print("Food Percentage: ");
  Serial.println(foodPercentageStr.c_str());

  if (foodPercentageInt <= 100 and foodPercentageInt >= 0){
    //mqtt publish food status
    mqtt.publish("b6310500287/food_level",foodPercentageStr.c_str());
  }
  else{
    Serial.println("Not publishing.");
  }
  
  
  delay(1000);
}