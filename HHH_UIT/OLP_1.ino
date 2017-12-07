// Import required libraries
#include "ESP8266WiFi.h"
#include "DHT.h"
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "WiFiManager.h"

// DHT11 sensor pins
#define DHTPIN 5
#define DHTTYPE DHT11
#define LEDPIN 4

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE, 15);

const char* device_id = "team06";

// WiFi parameters
const char* ssid = "Pyrus";
const char* password = "741456963";
const char* server = "192.168.43.61";

WiFiClient espClient;
PubSubClient mqttclient(espClient);
// Variables to be exposed to the API
float temperature;
float humidity;

void connect_wifi();
void callback(char* topic, byte* payload, unsigned int length);

void setup(void)
{  
  // Start Serial
  Serial.begin(115200);

//  WIFI-connect
  connect_wifi();

  // Init DHT 
  dht.begin();
  pinMode(LEDPIN, OUTPUT);

  //mqtt-init
  mqttclient.setServer(server,1883);
  reconnect();
  mqttclient.setCallback(callback);  
  mqttclient.subscribe("LED");
  Serial.println("Waiting...");
}

void loop() {
  reconnect();
  // Reading temperature and humidity
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  char buff_hu[10];
  dtostrf(humidity,0,0,buff_hu);

  char buff_te[10];
  dtostrf(temperature,0,0,buff_te);
  Serial.print("HUD:");
  Serial.println(humidity);
  Serial.print("TEM:");
  Serial.println(temperature);

  reconnect();
  mqttclient.publish("TEMP",buff_te); 
  mqttclient.publish("HUMI",buff_hu); 
  
  mqttclient.loop();
  delay(1000);
}

void connect_wifi(){
  Serial.print("Connecting to ");

  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!mqttclient.connected()) {
    Serial.print("Trying MQTT connection...");
    if (mqttclient.connect(device_id)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.println(mqttclient.state());
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length)
{
  String inString="";
  for (int i = 0; i < length; i++) {
    inString += (char)payload[i];
  }
  if(inString == "ON"){
    digitalWrite(LEDPIN, HIGH);  
    Serial.println("TRUE");    
  }
  if(inString == "OFF"){
    digitalWrite(LEDPIN, LOW);
    Serial.println("False");    
  }  
}


