void initmDNS() {
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
  int n = MDNS.queryService("mqtt", "tcp"); // Send out query for esp tcp services
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
}

