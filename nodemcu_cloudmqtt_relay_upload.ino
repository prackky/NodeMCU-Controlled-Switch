#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
 
// Connect to the WiFi
const char* ssid = "YOUR_WIFI_SSID"; // wifi SSID
const char* password = "YOUR_WIFI_PASSWORD"; // wifi password
const char* mqtt_server = "m45.cloudmqtt.com"; // change this with your server name
const char* device_id = "uniqueID"; // write any unique ID
 
WiFiClient espClient;
PubSubClient client(espClient);
 
//const byte ledPin3 = 0;
//const byte ledPin0 = 16;
const byte ledPin5 = 14;
//const byte ledPin6 = 12;
//const byte ledPin7 = 13;
//const byte ledPin8 = 15;
//const byte ledPin9 = 3;
//const byte ledPin10 = 1;
//const byte ledPin1 = 5;
//const byte ledPin2 = 4;
//const byte ledPin4 = 2;
//const byte ledPin12 = 10;
//const byte ledPin11 = 9;

char message_buff[100];
 
void callback(char* topic, byte* payload, unsigned int length) {
 //payload[length] = '\0';
 Serial.print("Message arrived [");
 Serial.print(topic);
 Serial.println("] ");
 int i;
 for(i=0; i<length; i++) {
    message_buff[i] = payload[i];
  }
  message_buff[i] = '\0';
  
  String msgString = String(message_buff);
  Serial.println(msgString);
 if (strcmp(topic, "uniqueID/topic")==0){ // write your unique ID here
    if(msgString == "1"){
    digitalWrite(ledPin5, LOW); // PIN HIGH will switch OFF the relay
    }
    if(msgString == "0"){
    digitalWrite(ledPin5, HIGH); // PIN LOW will switch ON the relay
    }
  }

}
 
void reconnect() {
 // Loop until we're reconnected
 while (!client.connected()) {
 Serial.print("Attempting MQTT connection...");
 // Attempt to connect
 if (client.connect(device_id, "cloud_username", "cloud_password")) { // enter cloudmqtt provided username and password in console
  Serial.println("connected");
  // ... and subscribe to topic
  client.subscribe("uniqueID/topic"); // write your unique ID here
 } else {
  Serial.print("failed, rc=");
  Serial.print(client.state());
  Serial.println(" try again in 5 seconds");
  // Wait 5 seconds before retrying
  delay(5000);
  }
 }
}
 
void setup()
{
 Serial.begin(115200);
 
 client.setServer(mqtt_server, 1883);
 client.setCallback(callback);
 
 pinMode(ledPin5, OUTPUT);
}
 
void loop()
{
 if (!client.connected()) {
  reconnect();
 }
 client.loop();
}
