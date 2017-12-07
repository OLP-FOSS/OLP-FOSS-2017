#include <ESP8266WiFi.h>
#include<PubSubClient.h>
#include "wifi_info.h"
#include <qrcode.h>
#include <SSD1306.h>
#include <Servo.h>


SSD1306  display(0x3c, 4, 5);
QRcode qrcode (&display);

//Khai báo SSID va PASS Wifi
const char* ssid = SSID;
const char* password = WPA_KEY;

//Khai báo địa chỉ MQTT Server
const char* mqtt_server= IP_MQTT;

//Khai báo biến ở đây .................................................................................................
//.....
const int pinLed = 16;
const int pinServo = 2;
const int pinSound = 14;
bool countdown = false;
int timecountdown = 0;
//const int trig = 8;     // chân trig của HC-SR04
//const int echo = 7;     // chân echo của HC-SR04

Servo myservo1;

//Khai báo WifiCLient
WiFiClient espclient;  

//Khai báo client trong thư viện PubSubClient để giao tiếp MQTT Server
PubSubClient client(espclient);    

//SETUP()
void setup() {
//  Set pinMode ở đây .................................................................................................
//  ...
  pinMode(pinLed,OUTPUT);   
  pinMode(pinServo,OUTPUT);   
  pinMode(pinSound,OUTPUT);
  digitalWrite(pinSound,HIGH);
//  pinMode(trig,OUTPUT);   // chân trig sẽ phát tín hiệu
//  pinMode(echo,INPUT);    // chân echo sẽ nhận tín hiệu
  myservo1.attach(pinServo);
  myservo1.write(0);
  
//  Set Baurate
  Serial.begin(115200);         

  display.init();
  display.clear();
  display.display();

//  Initialize QRcode display using library
  qrcode.init();
//  create qrcode
  qrcode.create("https://hocwebchuan.com/reference/tools/count_character.php");
  

//  Thiết lập kết nối Wifi
  setup_wifi();                                                       

//  Thiết lập kết nối đến MQTT server và hàm callback để lấy dữ liệu trả về từ server cho client
  client.setServer(mqtt_server, 1883);                                    
  client.setCallback(callback);                                          

//  Subcribe Topic trên MQTT Server
  listenMQTT();                                                     
}

//SETUP WIFI
void setup_wifi(){   
//  Hiển thị Serial Monitor
  Serial.print("Connecting to ");                                        
  Serial.println(ssid);
   
//  Kết nối đến AcceptPoint thông qua ssid và password
  WiFi.begin(ssid,password);                                  

//  Kiểm tra nếu kết nối FAIL - reconnect
  while(WiFi.status()!=WL_CONNECTED){                                     
    delay(200);
    Serial.println("Can't connect. Reconnecting...\n");
  }
//  Nếu kết nối thành công - Hiển thị ra Serial Monitor
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());                                        
}

//LISTENMQTT()- Kiểm tra kết nối Wifi , MQTT Server , subcribe topic 
void listenMQTT(){
//  Nếu không kết nối được wifi -> loop white
  while(WiFi.status()!=WL_CONNECTED){
    Serial.print("Can't connect. Reconnecting...\n");
    setup_wifi();                                                             
  }
//  //Subcribe topic ở đây .................................................................................................
  while(!client.connected()){
    //OSSTeamHDH - định danh cho thiết bị - mỗi thiết bị có 1 định danh riêng
    if(client.connect("OSS-ChimCanhCut")){
      Serial.println("Connected to MQTT Server"); 
      client.subscribe("open");                       
      Serial.println("Subscribe topic open success");  
   }
    else{
      Serial.print("Failed,Can't connect to MQTT Server , STATE = ");
      Serial.println(client.state());
      delay(100);
    }
  } 
}

//HANDLEPAYLOAD()- Nhận dữ liệu từ server trả về mảng char*
char* handlePayload(byte* payload,unsigned int lengthPayload ){
  char* charMESSAGE = new char[lengthPayload];  
  for(int i=0;i<lengthPayload;i++){                                          
      charMESSAGE[i] = (char)payload[i];                                          
    }
  return charMESSAGE;
}

//CALLBACK()
void callback(char* topic,byte* payload,unsigned int lengthPayload){  
//  Hiển thị Serial Monitor  
  Serial.print("Message arrived from topic : ");
  Serial.println(topic);

//  Hiển thị length của payload  
  Serial.print("Length of payload : ");
  Serial.println(lengthPayload);  
  
//  charMESSAGE đang giữ dữ liệu nhận được
  char* charMESSAGE = handlePayload(payload,lengthPayload);  
  if(lengthPayload > 0){
    Serial.print("MESSAGE RECEIVED IS : ");
    Serial.println(charMESSAGE);
//    Xử lý charMESSAGE ở đây .................................................................................................
    if(charMESSAGE[0] == 'C'){
      digitalWrite(pinLed,LOW);
      myservo1.write(180);
      countdown = true;
      timecountdown = 10000;
      Serial.println("Dien trong phong da bat");
    }      
  }
  else{
    Serial.print("Fail to receive payload !!! - payload : ");
    Serial.println(charMESSAGE);    
  } 
}

//LOOP()
void loop() {
//  Xử lý nội dung trong loop ở đây .................................................................................................
//  ......
  if(countdown == true){
    timecountdown -= 1000;
    Serial.print("timecountdown = ");
    Serial.println(timecountdown);
    if(timecountdown == 5000){
      digitalWrite(pinSound,LOW);
      Serial.println("pinsound - LOW");
      delay(300);
      digitalWrite(pinSound,HIGH);
      Serial.println("pinsound - HIGH");
      delay(300);
      digitalWrite(pinSound,LOW);
      Serial.println("pinsound - LOW");
      delay(300);
      digitalWrite(pinSound,HIGH);
      Serial.println("pinsound - HIGH");
    }
    if(timecountdown == 0){
      digitalWrite(pinLed,HIGH);
      myservo1.write(0);
      countdown = false;
      Serial.println("Dien trong phong da tat");
    }     
  }
  
//  Publish ở đây ...............................................................................
//  if(client.connect("ChimCanhCut")){     
//     client.publish("event-temp",temp);
//     client.publish("event-humi",humi);       
//  }  
//  else{
//    Serial.println("Can't send message to MQTT Server");
//  } 

//  Kiểm tra kết nối đến MQTT Server và luôn lắng nghe dữ liệu trả về 
//  Nếu mất kết nối -> Kết nối lại / Nếu kết nối ổn định luôn lắng nghe dữ liệu trả về 
  if(!client.connected()){       
     listenMQTT();
  }
  else{
    client.loop();
  }    
  delay(1000);
}
