#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include "DHT.h"

// ======= Thay đổi các thông tin ở đây =========

const char* default_mqtt_server = "192.168.43.216";
const char* default_mqtt_port = "1883";
const char* device_id = "team02";

// ==============================================

char mqtt_server[255];
char mqtt_port[6];
const char* channel_in_postfix = "/in";
const char* channel_out_postfix = "/out";
String channel_in = "esp/", channel_out = "esp/";

//Config AP
const char* ssidAP      = "ESP8266_team02";
const char* passwordAP  = "123456789";

// Initialize DHT sensor
const int DHTPIN = 2;       //Đọc dữ liệu từ DHT11 ở chân A0 trên ESP
const int DHTTYPE = DHT11;  //Khai báo loại cảm biến, có 2 loại là DHT11 và DHT22
DHT dht(DHTPIN, DHTTYPE); // 11 works fine for ESP8266

float humidity, temp_c;   // Values read from sensor
// Generally, you should use "unsigned long" for variables that hold time
unsigned long previousMillis = 0;        // will store last temp was read
const long cnt_sensor = 2000;              // interval at which to read sensor - See more at: http://www.esp8266.com/viewtopic.php?f=29&t=8746#sthash.IJ0JNSIx.dpuf
unsigned long preTimer = 0;
const long interval = 5000;             // interval to publish data each time.

int val = 0;
int pirState = LOW; // Start with no caution

WiFiClient espClient;
PubSubClient client(espClient);
char hostString[16] = "ABC";
char IP_Server_char[20];
int Port_Server;
IPAddress ip;

//Speaker Pin
#define SPEAKER_PIN 8

void setup() {
  Serial.begin(115200);
  
  //init Pin In/Out
  initPin();
  
  //Init WiFiManager
  initWiFiManager();

//  //init mDNS
//  //initmDNS();
 
//  //init MQTT
  initMQTT();
  Serial.println("Setup Successful");
}

void loop() {
  loopMQTT();
}



//////////////////////////////PUB/////////////////


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    //    if (client.connect("ESP8266Client")) {
    if (client.connect(device_id)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //      client.publish("ESP8266/connection status", "Connected!");
      //      client.publish(channel_out.c_str(), device_id);
      // ... and resubscribe
      //      client.subscribe("ESP8266/LED status");
      client.subscribe(channel_in.c_str());
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//////////////////////// Virtual output//////////////////
inline unsigned char Virtual_U1() {
  getTemperature();
  return temp_c;
}

inline unsigned char Virtual_U2() {
  getTemperature();
  return humidity;
}

inline unsigned char Virtual_U3() {
  return 1;
}

inline unsigned char Virtual_U4() {
  return 1;
}

inline unsigned char Virtual_U5() {
  return 1;
}

/////////////////////////////// Function input/////////////////////
inline void Function_F1() {
  Serial.println("F1");
  humidity = dht.readHumidity();
}

//Open door
inline void Function_F2() {
  Serial.println("F2");
}

inline void Function_F3() {
  Serial.println("F3");
}
inline void Function_F4() {
  Serial.println("F4");
}

inline void Function_F5() {
  Serial.println("F5");
}

/*==================== Setup ============================*/
