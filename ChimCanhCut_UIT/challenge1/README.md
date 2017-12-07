# 35 Alert System
Một hệ thống sử dụng ESP8266 để cảnh báo khi nhiệt độ cao quá ngưỡng 35 độ.

# Giới thiệu 
Hệ thống cảnh báo nhiệt độ cao.
- Có dashboard hiển thị nhiệt độ
- Có dashboard hiển thị đồ thị biến thiên nhiệt độ
- Có thông báo tooltips trên dashboard 
- Có thông báo qua bot Telegram

# Cài đặt
Nhóm đã đóng gói bằng docker để mọi người có thể thuận tiện sử dụng code

## Node-RED Docker
```
docker build --name chimcanhcut01 .
```

```
docker run -p 1883:1883 chimcanhcut01
```

## MQTT Broker Mosquitto
```
docker run -it -p 1883:1883 eclipse-mosquitto
```

## ESP8266
bạn nạp `esp8266-src/main.ino` vào mạch