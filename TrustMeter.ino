// Ver 4.2 - 10.2.22

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
Adafruit_SSD1306 Oled = Adafruit_SSD1306(138, 32, &Wire);

#include <EEPROM.h>
//#include "TFT.h"

byte laserPin = 0;      //Считываем с лазера
byte pwmPin = 3;        //ШИМ пин + 2
byte switchPin = 5;     //свич +/-  3
byte tick = 0;

int dt = 5;  // Время интегрирования ПИД регулятора
int mode = 1; // Стартовый мод программы
int switchDelay = 1000; // задержка в переключении полюсов

unsigned long timer = 0, timer1 = 0, timer_2 = 0;  //Таймеры millis
short del = 30; //Задержка вывода Info

float sinX = -1; // для генератора шума
float ratioFilter = 0.05; // коэффициент фильтрации 0.0-1.0
double laserLvl = 0, err = 0, errOld = 0, iNow = 0, iOld = 0, outPwm = 0;
double target = 450; // Целевое значение ПИД
double Kp = 0.5, Ki = 2, Kd = 0.4, P = 0, I = 0, D = 0; //пропорционельный, интегральный, дифференциальный коэф-ты (Считываются из еепром)

boolean show = 0, graph = 0, flagPass = 0, mesMan = 1, generationMeasure = 0;// Показывать значения, график,флаг для параметров, флаг сообщения о доступе, генерация шума измерений.

void setup() {

  //randomSeed(1);
  TCCR2B = (TCCR2B & 0xf8) | 1; //uno pin 3, разгон ШИМ до 32кГц
  //TCCR3A = (TCCR3A & 0xf8) | 1; //mega pin 2, 3, 5 разгон ШИМ до 32кГц
  //TCCR3A = 0b00000011;
  //TCCR3B = 0b00001001; // 0b00001001 - 15.6 kHz, 0b00000001 - 7.8 kHz
  TCCR2A = 0b00000011;
  TCCR2B = 0b00001001; // 0b00001001 - 15.6 kHz, 0b00000001 - 7.8 kHz
  Serial.begin(115200);
  Serial.setTimeout(50);
  Oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Oled.clearDisplay();
  Oled.setTextSize(1);
  Oled.setTextColor(SSD1306_WHITE);
  Oled.setCursor(0, 0);
  Oled.println("Display Online !");
  Oled.print(String(Oled.width()) + " X " + String(Oled.height()));
  Oled.display();
  delay(1000);

  pinMode(laserPin, INPUT);
  pinMode(pwmPin, OUTPUT);
  pinMode(switchPin, OUTPUT);

  //чтение еепром
  EEPROM.get(0, Kp); // 4 byte
  EEPROM.get(4, Ki);
  EEPROM.get(8, Kd);
  EEPROM.get(12, dt);
  EEPROM.get(16, target);
  EEPROM.get(20, ratioFilter);
  EEPROM.get(24, graph);
  EEPROM.get(28, switchDelay);

  if (graph == 1) {
    Serial.println("Start, for setup insert password /NNNN");
    Serial.println("Kp=" + String(Kp) + "; "
                   + "Ki=" + String(Ki) + "; "
                   + "Kd=" + String(Kd) + "; "
                   + "dt=" + String(dt) + "; "
                   + "switch=" + String(switchDelay) + "; "
                   + "target=" + String(target) + "; "
                   + "ratioFilter=" + String(ratioFilter) + "; ");
  }
 // start(); // изначальное расстояние
}

float expRunningAverage(float newVal, float ratio) {
  static float filVal = 450;
  filVal += (newVal - filVal) * ratio;
  return filVal;
}

void loop() {
  //pid();
  pidNew();
  info();
  parametres();
}
