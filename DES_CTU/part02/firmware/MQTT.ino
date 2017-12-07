void callback(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0'; // Null terminator used to terminate the char array
  String message = (char*)payload;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  for (int i = 0; i < length; i++) {  // print message converted from byte to char
    Serial.print((char)payload[i]);
  }
  Serial.println();
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
        case 5: Function_F1(); break;
        case 6: Function_F2(); break;
        case 7: Function_F3(); break;
        case 8: Function_F4(); break;
        case 9: Function_F5(); break;
      }
    }
  }
}

void send_data() {
  int D5 = digitalRead(D5);
  int D6 = digitalRead(D6);
  int D7 = digitalRead(D7);
  int D8 = digitalRead(D8);
  int A0 = analogRead(A0);
  int U1 = Virtual_U1();
  int U2 = Virtual_U2();
  int U3 = Virtual_U3();
  int U4 = Virtual_U4();
  int U5 = Virtual_U5();
  String pubString = String(D5) + ';' + String(D6) + ';' + String(D7) + ';' + String(D8) + ';' + String(A0) + ';' + String(U1) + ';' + String(U2) + ';' + String(U3) + ';' + String(U4) + ';' + String(U5) ;
  
  unsigned long curTimer = millis();
  if (curTimer - preTimer >= interval)
  {
    preTimer = curTimer;
    client.publish(channel_out.c_str(), pubString.c_str());
    Serial.print("Message published [");
    Serial.print(channel_out);
    Serial.print("]: ");
    Serial.println(pubString);
  }
}

void initMQTT() {
  Serial.print("mqtt server: ");
  Serial.println(mqtt_server);
  Serial.print("mqtt port: ");
  Serial.println(atoi(mqtt_port));
  
  Serial.println("connected...yeey :)");
  Serial.println("local ip");
  Serial.println(WiFi.localIP());
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
  client.setCallback(callback);
}

void loopMQTT() {
  if (!client.connected())
    reconnect();
  client.loop();
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
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
