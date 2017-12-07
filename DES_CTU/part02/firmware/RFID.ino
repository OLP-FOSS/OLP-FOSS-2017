void initRFID() {
  SPI.begin(); 
  rfid.PCD_Init();
}

void loopRFID() {
  // Look for new cards
  if (!rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if (!rfid.PICC_ReadCardSerial())
    return;
  
  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

// Check is the PICC of Classic MIFARE type
  if(piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }


  Serial.println(F("Card has been detected."));
   
  Serial.println(F("The NUID tag is:"));
  Serial.print(F("In hex: "));
  printHex(rfid.uid.uidByte, rfid.uid.size);
  Serial.println();

  Serial.println("Send RFID Data To Server");
  String data = "";
  for(int i=0; i<4; i++)
    data += rfid.uid.uidByte[i];
     
  client.publish("esp/team02/out", data.c_str());
  
  
//  //Clear data
//  for (byte i = 0; i < 4; i++) {
//     idTag[i] = 0xFF;
//  }

  //Halt PICC
  rfid.PICC_HaltA();

  //Stop encryption on PCD
  rfid.PCD_StopCrypto1();

  
}

void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
