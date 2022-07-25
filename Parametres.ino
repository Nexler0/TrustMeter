void parametres() {

  if (Serial.available() > 1) {
    char mes = Serial.read();
    int pass;    
    if (mes == '/') {
      pass = Serial.parseInt();
      if (pass == 1408) {
        flagPass = 1;
        if (mesMan == 1) {
          Serial.println("Access Granted");
          mesMan = 0;
          //listOperations();
        }
      }
    }
    if (flagPass == 1) {
      if      (mes == 'p' || mes == 'P') Kp = Serial.parseFloat();
      else if (mes == 'i' || mes == 'I') Ki = Serial.parseFloat();
      else if (mes == 'd' || mes == 'D') Kd = Serial.parseFloat();
      else if (mes == 't' || mes == 'T') dt = Serial.parseInt();
      else if (mes == 'z' || mes == 'Z') switchDelay = Serial.parseInt();
      else if (mes == 'x' || mes == 'X') target = Serial.parseInt();
      else if (mes == 'm' || mes == 'M') mode = Serial.parseInt();
      else if (mes == 's' || mes == 'S') show = Serial.parseInt();
      else if (mes == 'n' || mes == 'N') ratioFilter = Serial.parseFloat();
      else if (mes == 'g' || mes == 'G') graph = Serial.parseInt();
      else if (mes == 'e') {
        pass = 0;
        flagPass = 0;
      }
      else;
      listOperations();

      EEPROM.put(0, Kp); // 4 байта, запись в еепром ограничена
      EEPROM.put(4, Ki);
      EEPROM.put(8, Kd);
      EEPROM.put(12, dt);
      EEPROM.put(16, target);
      EEPROM.put(20, ratioFilter);
      EEPROM.put(24, graph);
      EEPROM.put(28, switchDelay);

      if (graph < 1 && show < 1) {
        Serial.println(" Kp = " + String(Kp, 2)
                       + " Ki= " + String(Ki, 2)
                       + " Kd= " + String(Kd, 2)
                       + " dt= " + String(dt, DEC)
                       + " switch= " + String(switchDelay, DEC)
                       + " target= " + String(target, 0)
                       + " mode= " + String(mode, DEC)
                       + " show= " + String(show, DEC)
                       + " ratioFilter= " + String(ratioFilter, DEC)
                       + " graph= " + String(graph, DEC));
      }
    } else {
      Serial.println("Wrong password");
    }
  }
}

void listOperations() {
  Serial.println("\n MENU"
                 + String("\n For setup press a letter and insert meaning. Example: p5.")
                 + String("\n Kp - pN")
                 + String("\n Ki - iN")
                 + String("\n Kd - dN")
                 + String("\n dt - tN")
                 + String("\n Switch - zN")
                 + String("\n Target - xN")
                 + String("\n Mode - mN")
                 + String("\n Show - sN")
                 + String("\n Ratio filter 0.0..1.0 - nN")
                 + String("\n Graphics - gN")
                 + String("\n Where N is a meanings")
                 + String("\n ee - Exit"));
  Serial.println("");
}
