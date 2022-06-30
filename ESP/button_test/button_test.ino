void setup() {
  
  Serial.begin(74880);
  pinMode(14,INPUT);

}

void loop() {
  Serial.println(digitalRead(14));
  delay(100);
}
