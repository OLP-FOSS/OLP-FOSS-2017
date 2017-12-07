void callback(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0'; // Null terminator used to terminate the char array
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
        case 5: Function_F1(); break;
        case 6: Function_F2(); break;
        case 7: Function_F3(); break;
        case 8: Function_F4(); break;
        case 9: Function_F5(); break;

      }
    }
  }
}
