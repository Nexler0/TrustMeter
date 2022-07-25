void pidNew() {
  float t = dt / 1000.0;

  if (millis() - timer >= dt) {
    timer = millis();
    if (generationMeasure) {
      laserLvl = sin(sinX) * random(70) + target;
      if (sinX == 1) {
        sinX = -1;
      }  else sinX += 0.05;
    } else laserLvl = expRunningAverage(analogRead(laserPin), ratioFilter);
    if (laserLvl < 200) laserLvl = 200;
    err = target - laserLvl;
    P = Kp * err;
    I = iOld + ((Ki * err) * t);
    if (I > 1023) {
      I = 1023;
    } else if (I < -1023) {
      I = -1023;
    }
    D = Kd * ((err - errOld) / t);
    outPwm = (P + I + D);
    outPwm = constrain(outPwm, -1023, 1023); //(-1023, 1023)

    short pwmInfo;
    if (millis() > 1000) { // для выхода на полку после рестарта без рывка
      if (outPwm >= 0 && outPwm <= 1023) {
        if (outPwm >= 767 && outPwm < 769) {
          outPwm = 769;
        }
      }
      else if (outPwm < 0 && outPwm >= -1023) {
        if (outPwm > -769 && outPwm <= -767) {
          outPwm = -769;
        }
      }
      
      if (laserLvl < target || outPwm > 0) {
        digitalWrite(switchPin, HIGH);
        analogWrite(pwmPin, 1023 - outPwm);
      } else if (laserLvl >= target || outPwm <= 0) {
        digitalWrite(switchPin, LOW);
        analogWrite(pwmPin, outPwm + 1);
      }
    }
  }

  if (graph) {
    Serial.println(String(target, 2)
                   + " + " + String(1050)
                   + " 0 " + String(0)
                   + " - " + String(-1050)
                   //+ " laserLvl " + String(laserLvl, 2)
                   + " outPwm " + String(outPwm, 2)
                   //+ " LaserLvl+outPwm " + String(laserLvl + outPwm, 2)
                   + " LaserPin " + String(analogRead(laserPin))
                   //+ " Target+outPwm " + String(target + outPwm, 2)
                   + " laserLvl " + String(laserLvl)
                  );
  }
  iOld = I;
  errOld = err;
  static unsigned long timerDisp;
  if (millis() - timerDisp >= 70) { // меньше 70 будет тормозить ПИД
    Oled.display();
    Oled.clearDisplay();
    Oled.setCursor(0, 0);
    Oled.setTextSize(1);
    Oled.setTextColor(SSD1306_WHITE);
    if (outPwm >= 1022 || outPwm <= -1022) {
      Oled.print("OutPwm   ");
      Oled.println("OVERLOAD");
    } else {
      Oled.print("OutPwm   ");
      Oled.println(outPwm, 2);
    }
    Oled.print("LaserLvl ");
    Oled.println(laserLvl, 2);
    Oled.println("P " + String(Kp) + " I " + String(Ki) + " D " + String(Kd));
    Oled.println("t " + String(dt) + " x " + String(target, 0) + " z " + String(switchDelay) + " " + (millis() - timerDisp));
    timerDisp = millis();
    /*    static int xPwm = 0, yPwm = 0;
        xPwm += 1;
        yPwm = map(outPwm, -1023, 1023, 31, 15);
        Serial.println("xPwm = " + String(xPwm) + " yPwm = " + String(yPwm));
        Oled.drawPixel(xPwm, yPwm,  SSD1306_WHITE);
        if (xPwm == 128) {
          xPwm = 0;
          Oled.clearDisplay();
        }
    */
  }
}
