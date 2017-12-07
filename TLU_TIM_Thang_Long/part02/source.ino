/* Uncomment the following if using MCP3008: define variable starting with MCP, MCP variable and mcpRead function. */
/* Uncomment the following if using 74HC4607: define variablee starting with HC4607, HC4067Read function. */
#include <Servo.h>
#include               <DHT.h>
#include       <ESP8266WiFi.h>
#include       <WiFiManager.h>
#include      <PubSubClient.h>
#include <ESP8266HTTPClient.h>
#define MQTT_SERVER              "172.20.10.6"
#define MQTT_PORT                       "1883"
#define HOME_WiFi                     "Team11"
#define PASSWORD                   "123321789"
#define DHTSensor                        DHT11
#define DHTPin                              D3
#define Vref                               3.3
#define device_id "team11"
#define echoPIN                             D7
#define trigPIN                             D2
#define SERVO_PIN 4 
Servo gServo;
bool usingbyRainSensorON = 0;
bool usingbyRainSensorOFF = 0;
bool usingbyLightSensor = 0;
// Generally, you should use "unsigned long" for variables that hold time
const long cnt_sensor = 2000;              // interval at which to read sensor - See more at: http://www.esp8266.com/viewtopic.php?f=29&t=8746#sthash.IJ0JNSIx.dpuf
unsigned long preTimer = 0;
unsigned long previousMillis = 0;        // will store last temp was read
const long cnt_sensor2 = 2000;              // interval at which to read sensor - See more at: http://www.esp8266.com/viewtopic.php?f=29&t=8746#sthash.IJ0JNSIx.dpuf
unsigned long previousMillis2 = 0;        // will store last temp was read
const long interval = 5000;

String topicOUT= "esp/" + String(device_id) + "/out";
String topicIN= "esp/" + String(device_id) + "/in";
  

WiFiClient           espClient;
PubSubClient client(espClient);
DHT     dht(DHTPin, DHTSensor);
byte               lastCommand;
unsigned long     lastTime = 0;
char            incomeMSG[100];
                                        
void setup() {
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, INPUT);
  pinMode(D6, INPUT);
  pinMode(D7, INPUT);
  pinMode(D8, INPUT);
  pinMode(A0, INPUT);
  Serial.begin(115200);
  setup_wifi();
  gServo.attach(SERVO_PIN); 
  WiFiManager wifiManager;
  //wifiManager.resetSettings();
  wifiManager.setAPCallback(configModeCallback);
  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", MQTT_SERVER, 25);
  WiFiManagerParameter custom_mqtt_port("port", "mqtt port", MQTT_PORT, 4);
  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_mqtt_port);
  if(!wifiManager.autoConnect()) {
    Serial.println("failed to connect and hi~t timeout");
    ESP.reset();
    delay(1000);
  }
  client.setServer(MQTT_SERVER, String(MQTT_PORT).toInt());  
  client.setCallback(callback);
  gServo.write(0);
  delay(5000);
}

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

void setup_wifi()
{
  Serial.println("Connecting to " + String(HOME_WiFi));
  WiFi.begin(HOME_WiFi,PASSWORD);
  while(WiFi.status() != WL_CONNECTED)
  { delay(500); Serial.print("."); }
  Serial.println(" WiFi Connected");
}

void callback(char* topic, byte* payload, unsigned int length) {
  //payload[length] = '\0'; // Null terminator used to terminate the char array
  String message = (char*)payload;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  for (int i = 0; i < length; i++) {  // print message converted from byte to char
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
        case 5: Virtual_U1(); break;
        case 6: Virtual_U2(); break;
        case 7: Virtual_U3(); break;
        case 8: Virtual_U4(); break;
        case 9: Virtual_U5(); break;
      }
    }
  }

}

void sendToServer()
{  
  int _D5 = digitalRead(D5);
  int _D6 = digitalRead(D6);
  int _D7 = digitalRead(D7);
  int _D8 = digitalRead(D8);
  int _D9 = analogRead(A0);
  int U1 = Virtual_U1();
  int U2 = Virtual_U2();
  int U3 = Virtual_U3();
  int U4 = Virtual_U4();
  int U5 = Virtual_U5();  //co xe cho thue khong?
  String pushData = String(_D5) + ";" +String(_D6) + ";" +String(_D7) + ";" +String(_D8) + ";" +String(_D9) + ";" +String(U1) + ";" +String(U2) + ";" +String(U3) + ";" +String(U4) + ";" +String(U5);
  Serial.println(pushData);
  if (client.publish(topicOUT.c_str(), pushData.c_str()))
  {
    Serial.println("Send OK.");
  }
  else
    Serial.println("Error, can't send data to server.");
  client.subscribe(topicIN.c_str());
  return;
}

float gettemperature() {
  // Wait at least 2 seconds seconds between measurements.
  // if the difference between the current time and last time you read
  // the sensor is bigger than the interval you set, read the sensor
  // Works better than delay for things happening elsewhere also
  unsigned long currentMillis = millis();
float temp_c = 0;
  if (currentMillis - previousMillis >= cnt_sensor) {
    // save the last time you read the sensor
    previousMillis = currentMillis;

    // Reading temperature for humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
    temp_c = dht.readTemperature();     // Read temperature as Celcius
    // Check if any reads failed and exit early (to try again).
    /*    if (isnan(humidity) || isnan(temp_c)) {
          Serial.println("Failed to read from DHT sensor!");
          return;
        }*/
  }
  return temp_c;
}
    
float gethumidity() {
  // Wait at least 2 seconds seconds between measurements.
  // if the difference between the current time and last time you read
  // the sensor is bigger than the interval you set, read the sensor
  // Works better than delay for things happening elsewhere also
  unsigned long currentMillis = millis();
  float humidity = 0;
  if (currentMillis - previousMillis2 >= cnt_sensor2) {
    // save the last time you read the sensor
    previousMillis2 = currentMillis;

    // Reading temperature for humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
    humidity = dht.readHumidity();
    // Read humidity (percent)// Check if any reads failed and exit early (to try again).
    /*    if (isnan(humidity) || isnan(temp_c)) {
          Serial.println("Failed to read from DHT sensor!");
          return;
        }*/
  }
  return humidity;
}

//////////////////////// Virtual output//////////////////
inline unsigned char Virtual_U1() {
  float temp = gettemperature();
  if (temp > 35)
    digitalWrite(D0, HIGH);
  else
    digitalWrite(D0,LOW);
  return temp;
}

inline unsigned char Virtual_U2() {
  return gethumidity();
}

inline unsigned char Virtual_U3() {
  return 1;
} 

inline unsigned char Virtual_U4() {
  return 1;
}

inline unsigned char Virtual_U5() {
    unsigned long duration; // biến đo thời gian
    int distance;           // biến lưu khoảng cách
    
    digitalWrite(trigPIN,0);   // tắt chân trig
    delayMicroseconds(2);
    digitalWrite(trigPIN,1);   // phát xung từ chân trig
    delayMicroseconds(5);   // xung có độ dài 5 microSeconds
    digitalWrite(trigPIN,0);   // tắt chân trig
    
    // Đo độ rộng xung HIGH ở chân echo. 
    duration = pulseIn(echoPIN,HIGH);  
    // Tính khoảng cách đến vật.
    distance = int(duration/2/29.412);
    
    return distance;
}

/////////////////////////////// Function input/////////////////////
inline void Function_F1() {
  if (usingbyRainSensorOFF) 
  {
    gServo.write(180);
    usingbyRainSensorOFF = !usingbyRainSensorOFF;
  }
}

inline void Function_F2() {
  if (!usingbyRainSensorON) 
  {
    gServo.write(0);
    usingbyRainSensorON = !usingbyRainSensorON;
  }
}

// F1 vs F2 dong mo cua so, dua vao cam bien mua

inline void Function_F3() { 
  digitalWrite(D0, !usingbyLightSensor);
  usingbyLightSensor = !usingbyLightSensor;
}
inline void Function_F4() {
  Serial.println("F4");  
}

inline void Function_F5() {
  Serial.println("F5");
}

void reConnect()
{
  while(!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(device_id))
    {
      Serial.println("Connected");
      client.subscribe(topicIN.c_str());
    } else {
      Serial.println("failed, rc = " + String(client.state()) + " try again in 5 seconds"); 
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected())
    reConnect();
  client.loop();
  if ((unsigned long) (millis() - lastTime) > 5000)
  { 
    sendToServer();
    lastTime = millis();
  }
}
