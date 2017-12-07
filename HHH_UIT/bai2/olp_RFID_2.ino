#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <MFRC522.h>

WiFiClient espClient;
PubSubClient client(espClient);
//-------------------------------------------------
const char* ssid = "Pyrus";
const char* password = "741456963";
//------------------------------------------------
const char* mqtt_server = "192.168.43.61";
//-----------------------------------------------
const char* TOPIC_RFID = "RFID";
//---------------RFID-----------------------------
#define SS_PIN 15
#define RST_PIN 2

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.


void setup() 
{
  Serial.begin(115200);   // Initiate a serial communication
  setup_wifi(); 
  client.setServer(mqtt_server, 1883);
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  reconnect();
  //client.setCallback(callback);
}

//void callback(char* topic, byte* payload, unsigned int length)
//{
//  String inString="";
//  for (int i = 0; i < length; i++) {
//    inString += (char)payload[i];
//  }
//  if(inString == "ON"){
//    digitalWrite(5, HIGH);  
//    Serial.println("DOOR - TRUE");    
//  }
//  if(inString == "OFF"){
//    digitalWrite(5, LOW);
//    Serial.println("DOOR - False");    
//  }  
//}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  // kết nối đến mạng Wifi
  WiFi.begin(ssid, password);
  // in ra dấu . nếu chưa kết nối được đến mạng Wifi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // in ra thông báo đã kết nối và địa chỉ IP của ESP8266
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}

void reconnect() {
  // lặp cho đến khi được kết nối trở lại
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("Team06_ID_1")) {
      Serial.println("connected");
      // publish gói tin "Connected!" đến topic RFID
      client.publish("RFID", "Connected!");
      //client.subscribe("DOOR");

    } else {
      // in ra màn hình trạng thái của client khi không kết nối được với MQTT broker
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // delay 5s trước khi thử lại
      delay(5000);
    }
  }
}

void loop() 
{ 
  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();
  Serial.println("Send to MQTT server - RFID.");
  //client.publish(TOPIC_DISTANCE, "ON");
  client.publish("RFID",(char*) mfrc522.uid.uidByte);
  if (!client.connected()) {
    reconnect();
  }
  delay(3000);
} 
