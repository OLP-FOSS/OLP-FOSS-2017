#include <ESP8266WiFi.h>
#include<PubSubClient.h>
#include "wifi_info.h"
#include "DHT.h"         

//Khai báo SSID va PASS Wifi
const char* ssid = SSID;
const char* password = WPA_KEY;

//Khai báo địa chỉ MQTT Server
const char* mqtt_server= IP_MQTT;

//Khai báo biến ở đây .................................................................................................
//.....
const int DHTPIN = 5;       //Đọc dữ liệu từ DHT11 ở chân 2 trên mạch Arduino
const int DHTTYPE = DHT11;  //Khai báo loại cảm biến, có 2 loại là DHT11 và DHT22
const int pinLed = 4;



//Khai báo WifiCLient
WiFiClient espclient;  

//Khai báo client trong thư viện PubSubClient để giao tiếp MQTT Server
PubSubClient client(espclient);    

DHT dht(DHTPIN, DHTTYPE);
 

//SETUP()
void setup() {
  dht.begin();  
//  Set pinMode ở đây .................................................................................................
//  ...
  pinMode(DHTPIN,INPUT);
  pinMode(pinLed,OUTPUT);   
  
//  Set Baurate
  Serial.begin(115200);                                                  

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
//  while(!client.connected()){
//    //OSSTeamHDH - định danh cho thiết bị - mỗi thiết bị có 1 định danh riêng
//    if(client.connect("OSSTeamHDH")){
//      Serial.println("Connected to MQTT Server"); 
////      client.subscribe("ledControl");                       
////      Serial.println("Subscribe topic ledControl success");  
//   }
//    else{
//      Serial.print("Failed,Can't connect to MQTT Server , STATE = ");
//      Serial.println(client.state());
//      delay(100);
//    }
//  } 
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
//    .....

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
  float h = dht.readHumidity();    //Đọc độ ẩm
  float t = dht.readTemperature(); //Đọc nhiệt độ
  float f = dht.readTemperature(true); //Đọc nhiệt độ *F
  static char humi[10];
  static char temp[10];
  dtostrf(h,6,2,humi);
  dtostrf(t,6,2,temp);
  Serial.println();
  if ( t > 28 ){
    digitalWrite(pinLed, HIGH);
    Serial.print("Nhiet do lon hon ");
    Serial.print(t);
    Serial.println("*C --- Led on");
  }
  else{
    digitalWrite(pinLed, LOW);
    Serial.println("Led off");
  }
  
//  Publish ở đây ...............................................................................
  if(client.connect("ChimCanhCut")){     
     client.publish("event-temp",temp);
     client.publish("event-humi",humi);       
  }  
  else{
    Serial.println("Can't send message to MQTT Server");
  }
  Serial.print("Humidity: ");
  Serial.println(h);
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print("*C- ");
  Serial.print(f);
  Serial.println("*F ");

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
