void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A0));
}
bool systemON = false;
uint8_t fuelLevel = 100;
void loop() {
  uint32_t pgn = 0xFEF1;
  uint8_t sourceAddress = 0x01;
  uint16_t rpm = random(800, 3000);
  uint16_t temp = random(70, 240);
  uint8_t oilPressure = random(10, 80);
  uint8_t batteryVoltage = random(110, 140);
  uint8_t status = 0;
  static unsigned long lastDrain = millis();

  if (Serial.available()) {
    uint8_t cmd = Serial.read();
    if (cmd = 0xA1) {
      systemON  = true;
      fuelLevel - 100;
    }
    else if (cmd = 0xA2) {
      systemON = false;
    }
  }
  if (!systemON || fuelLevel == 0)
    return;


  if (temp > 200) status |= (1 << 0);
  if (oilPressure < 20) status |= (1 << 1);

  uint8_t data[8] = {
    lowByte(rpm), highByte(rpm), lowByte(temp), highByte(temp), oilPressure, batteryVoltage, fuelLevel, status
  };

  Serial.write(data, 8);
  delay(500);

  if (millis()- lastDrain > 3000) {
    lastDrain = millis();
    if (fuelLevel > 0) fuelLevel--;
  }
}

