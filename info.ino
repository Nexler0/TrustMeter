void info() {

  if (mode == 1) {
    if (millis() - timer1 >= del) {
      timer1 = millis();
      if (show == 1) {
        Serial.print("laserLvl: "); Serial.print(laserLvl, 1);
        Serial.print(" Err(t): "); Serial.print(err, 1);
        Serial.print(" Err(t)_1: "); Serial.print(errOld, 1);
        Serial.print(" P(t): "); Serial.print(P, 1);
        Serial.print(" I(t): "); Serial.print(I, 1);
        Serial.print(" I(t)_1: "); Serial.print(iOld, 1);
        Serial.print(" D(t): "); Serial.print(D, 1);
        Serial.print("  outPwm: "); Serial.println(outPwm, 1);
      }
    }
  }
}
