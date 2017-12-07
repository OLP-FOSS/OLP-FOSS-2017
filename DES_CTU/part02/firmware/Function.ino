inline void Function_F1() {
  Serial.println("F1");
}

//Open door
inline void Function_F2() {
  Serial.println("F2");
  //Active device
  digitalWrite(D0, HIGH);
  delay(1000);
  digitalWrite(D0, LOW);
}

inline void Function_F3() {
  Serial.println("F3");
}
inline void Function_F4() {
  Serial.println("F4");
}

inline void Function_F5() {
  Serial.println("F5");
}
