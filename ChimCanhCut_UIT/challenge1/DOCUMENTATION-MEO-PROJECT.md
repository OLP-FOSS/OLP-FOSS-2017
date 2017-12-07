# MEO Project
M.E.O là một dự án mã nguồn mở nhằm hỗ trợ kết nối 


# Table of Content
1. PubSubClient
2. DHT
3. WiFiManager

# 1. PubSubClient
## Bắt đầu cơ bản:
Thư viện này hỗ trợ cho người dùng publish/subscribe với một máy chủ hỗ trợ MQTT.
Xem một ví dụ đơn giản: File -> Example -> PubSubClient
Khai báo thư viện: #include <PubSubClient.h>

## Đầy đủ
### Constructor `PubSubClient()`:
Tạo một client chưa được kích hoạt và cài đặt cấu hình cơ bản:
EthernetClient ethClient;
PubSubClient client;

void setup() {
    client.setClient(ethClient);
    client.setServer("broker.example.com",1883);
    // client is now configured for use
}

### Constructor `PubSubClient(client)`:
Tạo một client và khởi tạo chưa đầy đủ:
EthernetClient ethClient;
PubSubClient client(ethClient);

void setup() {
    client.setServer("broker.example.com",1883);
    // client is now ready for use
}

### Constructor `PubSubClient(server, port, [callback], client, [stream])`:
Tạo một client với đầy đủ thông số bao gồm địa chỉ máy chủ, cổng để kết nối, tùy chọn con trỏ tới một hàm gọi lại tin nhắn, client và lưu trữ tin nhắn đã nhận sử dụng stream:

###`connect()`: 
Kết nổi tới client. Bao gồm các hàm:
boolean connect (clientID) : sử dụng ID người dùng kết nối tới máy chủ
boolean connect (clientID, willTopic, willQoS, willRetain, willMessage) : Kết nối client với một thông điệp Will sẽ được chỉ định.
boolean connect (clientID, username, password): sử dụng ID người dùng,  tên và mật khẩu kết nối tới máy chủ.
boolean connect (clientID, username, password, willTopic, willQoS, willRetain, willMessage): Kết nối client với một thông báo Will, tên người dùng và mật khẩu được chỉ định

###`disconnect()`:
Ngắt kết nối với client. 

###`publish()`:
publish một chuỗi tin nhắn đến topic được chỉ định. Bao gồm các function:
int publish (topic, payload)
int publish (topic, payload, retained)
int publish (topic, payload, length)
int publish (topic, payload, length, retained)
int publish_P (topic, payload, length, retained)

###subscribe (topic, [qos]): 
subcribe tin nhắn cho topic.

###`unsubscribe(topic)`:
unsubcribe từ topic được chỉ định

###`loop()`:
xử lý thư đến và duy trì kết nối

###`connected()`:
Kiểm tra client có còn được kết nối không



# 2. DHT_sensor_library
## Bắt đầu cơ bản
Thư viện này hỗ trợ cảm biến DHT11 , DHT22 , DHT21 . Đọc dữ liệu từ cảm biến truyền sang cổng Serial

Để đọc dữ liệu từ cảm biến
float h = dht.readHumidity();    
float t = dht.readTemperature();

## Đầy đủ
Constructor `DHT_sensor_library`:

### Hàm begin()
Bắt đầu đọc dữ liệu từ cảm biến

### Hàm readHumidity()
Đọc dữ liệu độ ẩm từ cảm biến trả về giá trị float

### Hàm readTemperature()
Đọc giá trị nhiệt độ (độ C) từ cảm biến trả về giá trị float

### Hàm readTemperature(true)
Đọc giá trị nhiệt độ (độ F) từ cảm biến trả về giá trị float

# 3. WiFiManager
## Bắt đầu cơ bản
Thư viện WiFiManager hỗ trợ kết nối wifi cho ESP dưới dạng giao diện. 
Khi ESP khởi động, nó sẽ thiết lập trong chế độ Station và cố kết nối với Access Point đã lưu trước đó.
Nếu không thành công (hoặc không có mạng lưu trước đó), nó sẽ chuyển ESP sang chế độ Access Point và WebServer (mặc định là ip 192.168.4.1).
Có thể sử dụng bất kỳ thiết bị có hỗ trợ wifi nào với trình duyệt (máy tính, điện thoại, máy tính bảng) kết nối với điểm truy cập mới được tạo ra.

### Khai báo các thư viện
include  < ESP8266WiFi.h > // ESP8266 Thư viện Core WiFi
include  < DNSServer.h >	// Máy chủ DNS cục bộ được sử dụng để chuyển hướng tất cả yêu cầu tới cổng cấu hình
include  < ESP8266WebServer.h >	// Local WebServer được sử dụng để phục vụ cổng cấu hình 
include  < WiFiManager.h > 
### Khai báo 
WiFiManager wifiManager;
### Một số hàm cơ bản
// tham số đầu tiên là tên điểm truy cập, thứ hai là mật khẩu 
wifiManager.autoConnect ( " AP-NAME " , " AP-PASSWORD " );
//nếu bạn chỉ muốn có một điểm truy cập 
wifiManager.autoConnect ( " AP-NAME " );
## Đầy đủ
Constructor `WiFiManager`:

### Hàm autoconnect()
Bạn có thể và nên bảo vệ mật khẩu cho điểm truy cập cấu hình. Chỉ cần thêm mật khẩu như một tham số thứ hai vào autoConnect. Hãy sử dụng một mật khẩu có chiều dài khoảng 8 ký tự trở lên.

wifiManager.autoConnect ( " AutoConnectAP " , " mật khẩu " )

Chỉ muốn có một điểm truy cập 

wifiManager.autoConnect ( " AP-NAME " );

Nếu bạn muốn sử dụng và tự động tạo ra tên từ 'ESP' và sử dụng Chip ID của ESP

wifiManager.autoConnect ();
### Hàm setAPCallback()
Sử dụng tính năng này nếu bạn cần làm điều gì đó khi thiết bị của bạn vào chế độ cấu hình khi kết nối WiFi không thành công. TrướcautoConnect()

wifiManager.setAPCallback (configModeCallback);

Ví dụ 

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  Serial.println(myWiFiManager->getConfigPortalSSID());
}

### Hàm setSaveConfigCallback()
Điều này được gọi khi các thông số tùy chỉnh đã được thiết lập VÀ kết nối đã được thiết lập. Sử dụng nó để thiết lập một cờ-FLAG, do đó, khi tất cả các cấu hình kết thúc, bạn có thể lưu các thông số thêm một nơi nào đó.

wifiManager.setSaveConfigCallback (saveConfigCallback);

Ví dụ

//flag for saving data
bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

## 