void setup(){
  Serial.begin(9600);
}

void loop(){
  Serial.println((int)analogRead(A0));
  delay(100);
}
