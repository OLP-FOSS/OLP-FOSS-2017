void initWiFiManager() {
  WiFiManagerParameter custom_text("<br/><p>Enter MQTT Server/IP and Port Number</p>");
  WiFiManagerParameter custom_mqtt_server("server", "mqtt_server", default_mqtt_server, 255);
  WiFiManagerParameter custom_mqtt_port("port", "mqtt_port", default_mqtt_port, 6);
  Serial.println("AAA");
  WiFiManager wifiManager;
  wifiManager.setAPStaticIPConfig(IPAddress(10,1,1,1), IPAddress(10,1,1,1), IPAddress(255,255,255,0));
  
  wifiManager.addParameter(&custom_text);
  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_mqtt_port);
  
//  wifiManager.resetSettings();
  if (!wifiManager.autoConnect(ssidAP, passwordAP)) {
    Serial.println("Failed to connect, we should reset as see if it connects");
    delay(3000);
    ESP.reset();
  }
  
  strcpy(mqtt_server, custom_mqtt_server.getValue());
  strcpy(mqtt_port, custom_mqtt_port.getValue());
}

