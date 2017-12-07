#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include "MFRC522.h"
#define RST_PIN  4  // RST-PIN für RC522 - RFID - SPI - Modul GPIO5 
#define SS_PIN  5  // SDA-PIN für RC522 - RFID - SPI - Modul GPIO4 

// Thông tin Wifi
const char* ssid = "Khanh Ney";
const char* password = "team3hutech1";

const char* mqtt_server = "iot.eclipse.org"; // Thông tin MQTT Broker Maker Hanoi đang deploy

unsigned long uidDec, uidDecTemp;
int led = 0;
int dem =0;
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance


// ClientID 
const char* clientID = "lamlong70";

//Thiết lập output xuất(public) và nhận(subscibe) dữ liệu với MQTT
const char* outTopic = "public/esp/outputLamLong";
const char* inTopic = "public/esp/inputLamLong";

unsigned long previousMillis = 0;        // will store last temp was read
const long interval = 2000;              // interval at which to read sensor
const int threshold = 600;
// thiết lập ngưỡng do chúng ta tự setup
WiFiClient espClient;
PubSubClient client(espClient);
char msg[50];
int count = 0;

// Hàm kết nối Wifi
void setup_wifi() {

  delay(250);
  
  SPI.begin();           // Init SPI bus
  mfrc522.PCD_Init();    // Init MFRC522
  
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  int retries = 0;
  while ((WiFi.status() != WL_CONNECTED) && (retries < 10)) {
    retries++;
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
//  pinMode(D2, OUTPUT);
  pinMode(led, OUTPUT);
//  khai báo sử dụng D2 làm gốc test
}

// Hàm call back được gọi khi nhận được thông tin từ subcribing topic
void callback(char* topic, byte* payload, unsigned int length) {
  // xử lý dữ liệu nhận được
  /*
   * convert string to int giá trị cường độ sáng
   * if/else điều khiển bật tắt relay dựa trên ngưỡng
  */
  char* message = (char*) payload;
  int value = atoi(message);
//  hàm convert string sang number thôi ae
  Serial.println(value);
  if (value > threshold) {
      digitalWrite(BUILTIN_LED, HIGH);
      Serial.println("HIGH");
      Serial.println(message);
  } else {
    digitalWrite(BUILTIN_LED, LOW); 
    Serial.println("LOW");
    Serial.println(message);
  }
}

// Reconnect đến MQTT Broker
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clientID)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish(outTopic, clientID);
      // ... and resubscribe
      client.subscribe(inTopic);
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
//vòng đời kiểm tra kết nối
void publishValue() {
  count++;
  dtostrf(count, 2, 2, msg);
  Serial.println(msg);
  dtostrf(analogRead(A0), 2, 2, msg);
  Serial.println(msg);
  client.publish(outTopic, msg);
  delay(2000);
//  nếu sử dụng public dữ liệu thì sau 2s sẽ gửi tín hiệu lên topic/server 1 lần
}
void RC255()
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    delay(50);
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }
  // Show some details of the PICC (that is: the tag/card)
  Serial.print(F("Card UID:"));
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println();

  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uidDecTemp = mfrc522.uid.uidByte[i];
    uidDec = uidDec*256+uidDecTemp;
  } 
  Serial.print("");
  Serial.print(uidDec);
  char buffer[10];
  dtostrf(uidDec,0, 0, buffer);
  client.publish(outTopic,buffer);
  digitalWrite(led, HIGH);
  delay(1500);
  digitalWrite(led, LOW); 
  Serial.println();
}
void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  //client.setCallback(callback);
}

// Helper routine to dump a byte array as hex values to Serial
void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    //Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  RC255();
//  publishValue();
  client.loop();
}
