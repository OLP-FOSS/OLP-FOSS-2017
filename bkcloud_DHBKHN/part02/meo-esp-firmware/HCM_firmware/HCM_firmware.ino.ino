#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include "DHT.h"
#include <ArduinoJson.h>

#include "SH1106.h"
SH1106  display(0x3c, 4, 5);
// ======= Thay đổi các thông tin ở đây =========

const char* default_mqtt_server = "server.tobernguyen.com";
const char* default_mqtt_port = "1883";
const char* device_id = "team01_HCM";

unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;

const long intervalSendData = 5000;
const long intervalSendRegister=10000;

// ==============================================

char mqtt_server[255];
char mqtt_port[6];
const char* channel_in_postfix = "/in";
const char* channel_out_postfix = "/out";
String channel_in = "esp/", channel_out = "esp/";
String hotspot_name_prefix = "ESP_8266_";

boolean isRegister=false;// trạng thái thiết bị đã đăng ký hay chưa
const char* topic_data="esp/data";
const char* topic_action="";
const char* topic_new="esp/newDevice";
const char* location_id="HCM";
const char* location_name="Ho Chi Minh";


// Initialize DHT sensor
const int DHTPIN = 14;       //Đọc dữ liệu từ DHT11 ở chân GPIO04 trên ESP
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
char hostString[16] = {0};
char IP_Server_char[20];
int Port_Server;
IPAddress ip;

//Led pin
int ledPin = 12;


void callback(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0'; // Null terminator used to terminate the char array
  String message = (char*)payload;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");

    if(strcmp(topic,topic_action)==0){

      payload[length] = '\0';
      char payload_string[200];
      strncpy(payload_string, (char*)payload,sizeof(payload_string));
      Serial.println(payload_string);
      StaticJsonBuffer<200> jsonBuffer;

      JsonObject& root = jsonBuffer.parseObject(payload_string);
      Serial.println(payload_string);
      const char* type = root["type"];

      const char* action = root["action"];

      if(strcmp(type,"register")==0){
        Serial.println("Register");
        const char* status1 = root["status"];
        if(strcmp(status1,"OK")==0){
          isRegister=true;
          Serial.println("Register Success");
        }
      }

      if(strcmp(type,"ledAction")==0){
        Serial.println("led");

        if(strcmp(action,"ON")==0){
          digitalWrite(ledPin,HIGH);
          Serial.println(action);
        }else{
          digitalWrite(ledPin,LOW);
        }
      }
//      sendStateSensor();mqtt_server
  }

  for (int i = 0; i < length; i++) {  //print message converted from byte to char
    Serial.print((char)payload[i]);
  }
  Serial.println(" ");
  int A[10][2];
  int index1 = 0, index2 = 0;
  int temp = 0;
  int prep = -1;
  for (int i = 0; i < length; i++)
  {
    char c = payload[i];
    if (c >= '0' && c <= '9') {
      temp = temp * 10 + (c - '0');
    }
    else if (c == '.') {
      A[index1][index2] = temp;
      temp = 0;
      index2 = (index2 + 1 ) % 2;
    }
    else if (c == ';')
    {
      if (i == prep + 1)
      {
        A[index1][0] = -1;
        A[index1][1] = -1;
      }
      else
      {
        A[index1][index2] = temp;

      }
      temp = 0;
      index2 = 0;
      index1 ++;
      prep = i;
    }

  }
  if (prep + 1 == length) {
    A[index1][0] = -1;
    A[index1][1] = -1;
  } else {
    A[index1][index2] = temp;
  }


  for (int i = 0; i < 10 ; i++)
  {
    if (A[i][0] == 0)
    {
      switch (i)
      {
        case 0:
          if (A[i][1] == 0)
            digitalWrite(D0, LOW);
          else
            digitalWrite(D0, HIGH);
          break;
        case 1:
          if (A[i][1] == 0)
            digitalWrite(D1, LOW);
          else
            digitalWrite(D1, HIGH); break;
        case 2:
          if (A[i][1] == 0)
            digitalWrite(D2, LOW);
          else
            digitalWrite(D2, HIGH); break;
        case 3:
          if (A[i][1] == 0)
            digitalWrite(D3, LOW);
          else
            digitalWrite(D3, HIGH); break;
        case 4:
          if (A[i][1] == 0)
            digitalWrite(D4, LOW);
          else
            digitalWrite(D4, HIGH); break;
      }
    }
    else if (A[i][0] == 1)
    {
      switch (i)
      {
        case 0: analogWrite(D0, A[i][1]); break;
        case 1: analogWrite(D1, A[i][1]); break;
        case 2: analogWrite(D2, A[i][1]); break;
        case 3: analogWrite(D3, A[i][1]); break;
        case 4: analogWrite(D4, A[i][1]); break;
        case 5: getTemperature(); break;
        case 6: getHumidity(); break;
        case 7: Function_F3(); break;
        case 8: Function_F4(); break;
        case 9: Function_F5(); break;

      }
    }
  }

}

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
      client.subscribe(topic_action);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void gettemperature() {
  // Wait at least 2 seconds seconds between measurements.
  // if the difference between the current time and last time you read
  // the sensor is bigger than the interval you set, read the sensor
  // Works better than delay for things happening elsewhere also
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= cnt_sensor) {
    // save the last time you read the sensor
    previousMillis = currentMillis;

    // Reading temperature for humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
    humidity = dht.readHumidity();          // Read humidity (percent)
    temp_c = dht.readTemperature();     // Read temperature as Celcius
    // Check if any reads failed and exit early (to try again).
    /*    if (isnan(humidity) || isnan(temp_c)) {
          Serial.println("Failed to read from DHT sensor!");
          return;
        }*/
  }
}


//////////////////////// Virtual output//////////////////
inline unsigned char Virtual_U1() {
//  gettemperature();
  return temp_c;
}

inline unsigned char Virtual_U2() {
//  gettemperature();
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
float getTemperature() {
  return dht.readTemperature();
}

float getHumidity() {
  return dht.readHumidity();
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
void setup() {
  hotspot_name_prefix += device_id;
  
  dht.begin();
  pinMode(D0, OUTPUT);    // Initialize the BUILTIN_LED pin as an output
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);

  pinMode(D5, INPUT);
  pinMode(D6, INPUT);
  pinMode(D7, INPUT);
  pinMode(D8, INPUT);
  pinMode(A0, INPUT);
  Serial.begin(115200);
  
  WiFiManager wifiManager;
  WiFiManagerParameter custom_text("<br/><p>Enter MQTT Server/IP and Port Number</p>");
  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", default_mqtt_server, 255);
  WiFiManagerParameter custom_mqtt_port("port", "mqtt port", default_mqtt_port, 6);
  wifiManager.addParameter(&custom_text);
  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_mqtt_port);
//  wifiManager.resetSettings();
  if (!wifiManager.autoConnect(hotspot_name_prefix.c_str())) {
    Serial.println("failed to connect, we should reset as see if it connects");
    delay(3000);
    ESP.reset();
    delay(5000);
  }
  pinMode(ledPin, OUTPUT);
  strcpy(mqtt_server, custom_mqtt_server.getValue());
  strcpy(mqtt_port, custom_mqtt_port.getValue());

  Serial.print("mqtt server: ");
  Serial.println(mqtt_server);
  Serial.print("mqtt port: ");
  Serial.println(atoi(mqtt_port));

  Serial.println("connected...yeey :)");
  Serial.println("local ip");
  Serial.println(WiFi.localIP());
  // ============== This section is for mDNS =================
  // setup_wifi();
  /*
  Serial.println("\r\nsetup()");
  sprintf(hostString, "ESP_%06X", ESP.getChipId());
  Serial.print("Hostname: ");
  Serial.println(hostString);
  WiFi.hostname(hostString);
  if (!MDNS.begin(hostString)) {
    Serial.println("Error setting up MDNS responder!");
  }
  Serial.println("mDNS responder started");
  //MDNS.addService("esp", "tcp", 8080); // Announce esp tcp service on port 8080

  Serial.println("Sending mDNS query");
  int n = MDNS.queryService("mqtt", "tcp"); // S#include "SH1106.h"
SH1106  display(0x3c, 4, 5);end out query for esp tcp services
  Serial.println("mDNS query done");
  if (n == 0) {
    Serial.println("no services found");
  }
  else {
    Serial.print(n);
    Serial.println(" service(s) found");
    //    for (int i = 0; i < n; ++i) {
    // Print details for each service found
    //      Serial.print(i + 1);
    //      Serial.print(": ");
    Serial.print(MDNS.hostname(0));
    Serial.print(" (");
    Serial.print(MDNS.IP(0));
    Serial.print(":");
    Serial.print(MDNS.port(0));
    Serial.println(")");
    }

  ///////////////// Get IP Adress and Port Adress/////////////
  IPAddress ip = MDNS.IP(0); // Get IPAddress of Server
  String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]); // convert IP Adress to String
  ipStr.toCharArray(IP_Server_char, 20);  // convert IP to char
  Serial.println(IP_Server_char);
  Port_Server = MDNS.port(0);
  Serial.println(Port_Server);
  //  client.setServer(IP_Server_char, Port_Server);
  */
  // ========================================================
  client.setServer(mqtt_server, atoi(mqtt_port));
  Serial.println();

  channel_in += device_id;          // "esp/NodeMcuEsp01"
  channel_in += channel_in_postfix; // "esp/NodeMcuEsp01/in"
  Serial.println("Subscribe channel: ");
  Serial.println(channel_in);

  channel_out += device_id;           // "esp/NodeMcuEsp01"
  channel_out += channel_out_postfix; // "esp/NodeMcuEsp01/out"
  Serial.println("Channel out: ");
  Serial.println(channel_out);

  Serial.println("loop() next");
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_24);
  client.setCallback(callback);
}

void loop() {
  String mac=WiFi.macAddress();
  String topic_action_string="esp/"+String(location_id)+"/action";
  topic_action=topic_action_string.c_str();
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long currentMillis = millis();
    if (currentMillis - previousMillis2 >= intervalSendRegister) {
    previousMillis2 = currentMillis;
    if(isRegister==false){
      Serial.println("sendRegister");
      sendRegister();
    }
    displayData();
  }
  if (currentMillis - previousMillis1 >= intervalSendData) {
    previousMillis1 = currentMillis;
//    displayData();
    if(isRegister==true){
      send_data();
    }

  }
}

//////////////////////////////PUB/////////////////
void send_data(){

//  int D5 = digitalRead(D5);
//  int D6 = digitalRead(D6);
//  int D7 = digitalRead(D7);
//  int D8 = digitalRead(D8);
//  int A0 = analogRead(A0);
//  int U1 = Virtual_U1();
//  int U2 = Virtual_U2();
//  int U3 = Virtual_U3();
//  int U4 = Virtual_U4();
//  int U5 = Virtual_U5();
//  String pubString = String(D5) + ';' + String(D6) + ';' + String(D7) + ';' + String(D8) + ';' + String(A0) + ';' + String(U1) + ';' + String(U2) + ';' + String(U3) + ';' + String(U4) + ';' + String(U5) ;
//
//  unsigned long curTimer = millis();
//  if (curTimer - preTimer >= interval)  // set time cycle to publish data: each 5s
//  {
//    preTimer = curTimer;
//    client.publish(channel_out.c_str(), pubString.c_str());
//    Serial.print("Message published [");
//    Serial.print(channel_out);
//    Serial.print("]: ");
//    Serial.println(pubString);
//  }

//  Serial.println(WiFi.macAddress());
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["location_id"] =location_id;
  root["type"] = "data";
//  root["location"] = location;
  JsonArray& sensorsData = root.createNestedArray("sensorsData");

  StaticJsonBuffer<100> jsonBuffer1;
  JsonObject& temperature = jsonBuffer1.createObject();
  temperature["name"] = "DHT11-t";
  if(isnan(getTemperature())){
    return;
  }else{
    temperature["value"] = getTemperature();
  }
  temperature["unit"] = "C";

  StaticJsonBuffer<100> jsonBuffer2;
  JsonObject& humidity = jsonBuffer2.createObject();
  humidity["name"] = "DHT11-h";
  if(isnan(getHumidity())){
    return;
  }else{
    humidity["value"] = getHumidity();
  }
  Serial.println(getHumidity());
  Serial.println(getTemperature());

  humidity["unit"] = "%";

  sensorsData.add(temperature);
  sensorsData.add(humidity);

  char dataMessage[200];
  root.printTo(dataMessage, sizeof(dataMessage));// Chuyen tu json sang String de pub
  client.publish(topic_data,dataMessage);
}

void sendRegister(){
  StaticJsonBuffer<300> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["location_id"] = location_id;
  root["location_name"] = location_name;
  root["type"] = "ESP8266";
  root["deviceID"] = "Team01";

  JsonArray& sensors = root.createNestedArray("sensors");

  StaticJsonBuffer<100> jsonBuffer1;
  JsonObject& temperature = jsonBuffer1.createObject();
  temperature["name"] = "DHT11-t";
  temperature["unit"] = "C";

  StaticJsonBuffer<100> jsonBuffer2;
  JsonObject& humidity = jsonBuffer2.createObject();
  humidity["name"] = "DHT11-h";
  humidity["unit"] = "%";

  sensors.add(temperature);
  sensors.add(humidity);

  char resiterMessage[200];
  root.printTo(resiterMessage, sizeof(resiterMessage));// Chuyen tu json sang String de pub
  Serial.println(resiterMessage);
  client.publish(topic_new,resiterMessage);
}

void displayData(){
    display.clear();
    Serial.println("display");
    float temp = getTemperature();
    float humi = getHumidity();
    display.drawString(0, 0, (String)location_name);
    
    display.drawString(0, 20, "T="+(String)temp +" C");
    display.drawString(0, 40, "H="+(String)humi +" %");
    display.display();
}

