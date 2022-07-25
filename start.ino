void start() {
  delay(5000);
  Oled.display();
  Oled.clearDisplay();
  Oled.setCursor (0, 0);
  target = analogRead(laserPin);
  if (target < 200){
    target = 200;
  }
  EEPROM.put(16, target);
  Serial.println(target);
  Oled.print("Start target value: \n \n     " + String(target));
  Oled.display();
  delay(2000);
}
