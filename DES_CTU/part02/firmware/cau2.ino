#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <DNSServer.h>
#include "DHT.h"
#include <SPI.h>
#include <MFRC522.h>

//Config network
const char* default_mqtt_server = "192.168.43.131";
const char* default_mqtt_port = "1883";
const char* device_id = "team02";

//Config AP
const char* ssidAP      = "ESP8266_team02";
const char* passwordAP  = "123456789";

char mqtt_server[255];
char mqtt_port[6];
const char* channel_in_postfix = "/in";
const char* channel_out_postfix = "/out";
String channel_in = "esp/", channel_out = "esp/";

//RFID
#define RST_PIN 4
#define SS_PIN  15
MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
byte idTag[4];

//Timer variable
unsigned long previousMillis = 0;        
const long cnt_sensor = 2000;              
unsigned long preTimer = 0;
const long interval = 5000;  
unsigned long currentTime = 0;
unsigned long previousTime =0;

WiFiClient espClient;
PubSubClient client(espClient);
char hostString[16] = {0};
char IP_Server_char[20];
int Port_Server;
IPAddress ip;

void setup() {
  Serial.begin(115200);
  initPin();
  initWiFiManager();
  Serial.println("Set up wifi successful");
  initRFID();
  Serial.println("Set up RFID successful");
  initMQTT();
  Serial.println("Set up MQTT successful");
}

void loop() {
  loopMQTT();
  loopRFID();
}



