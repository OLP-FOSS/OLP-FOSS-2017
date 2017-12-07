void getTemperature() {
  // Wait at least 2 seconds seconds between measurements.
  // if the difference between the current time and last time you read
  // the sensor is bigger than the interval you set, read the sensor
  // Works better than delay for things happening elsewhere also
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= cnt_sensor) {
    // save the last time you read the sensor
    previousMillis = currentMillis;
    
    // Reading temperature for humidity takes about 250 milliseconds!
    // Sensor readings may also be up tyo 2 seconds 'old' (it's a very slow sensor)
    temp_c = dht.readTemperature();     // Read temperature as Celcius
    // Check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(temp_c)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    if(temp_c > 30) {
      for(int i=0; i<=10; i++) {
        digitalWrite(D0, HIGH);
        delay(200);
        digitalWrite(D0, LOW);
        delay(200);
      }
    }
  }
}

void getHumidity() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= cnt_sensor) {
    // save the last time you read the sensor
    previousMillis = currentMillis;
    
    humidity = dht.readHumidity();
    // Check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(temp_c)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }
  }
}

