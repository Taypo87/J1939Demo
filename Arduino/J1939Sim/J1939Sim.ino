void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A0));
}

void loop() {
  uint32_t pgn = 0xFEF1;
  uint8_t sourceAddress = 0x01;
  uint16_t rpm = random(800, 3000);
  uint8_t data[8] = {
    lowByte(rpm), highByte(rpm), 0, 0, 0, 0, 0, 0
  };
  Serial.print(String(pgn, HEX));
  Serial.print(",");
  Serial.print(String(sourceAddress, HEX));
  for (int i = 0; i < 8; i++) {
    Serial.print(",");
    Serial.print(String(data[i], HEX));
  }
  Serial.println();
  delay(500);
}
