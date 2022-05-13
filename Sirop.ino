#include "GyverTimer.h"
#include <GyverButton.h>
#include <EEPROM.h>
GTimer GTimerflash1(MS);
GTimer GTimerflash2(MS);
GTimer Click(MS);
GTimer Click2(MS);
GTimer sirop1(MS);
GTimer sirop2(MS);
GTimer micdelay(MS);
GTimer GTimerback(MS);
//int led1 = 8;
int led2 = 9;
//int led3 = 7;   
int led4 = 6;
int rel1 = 11;
int rel2 = 12;
int a = 0; //переменная состояния 0-standby, 1-выбор 1 сиропа, 2-выбор 2 сиропа, 3 - тест, 4 - программирование, 5 и 6 - расчет времени.
int b = 0; // переменная состояния микирика.
#define BUTT1 4
#define BUTT2 5
#define MICKR 3
#define TESTS 2
#define PROGR 10
GButton but1(BUTT1);
GButton but2(BUTT2);
GButton mic(MICKR);
GButton test(TESTS);
GButton prog(PROGR);

int sirop1time;
int sirop2time;

const int DelayFlash = 1000;
const int buttTimeout = 30000;
//const int backTimeout = 10000;
uint32_t myTimer1;
uint32_t myTimer2;

void setup() {
  EEPROM.get(4, sirop1time);
  EEPROM.get(3, sirop2time);
  //pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
 // pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(rel1, OUTPUT);
  pinMode(rel2, OUTPUT);

  //digitalWrite(led1, LOW);
  digitalWrite(led2, HIGH);
  //digitalWrite(led3, LOW);
  digitalWrite(led4, HIGH);

  digitalWrite(rel1, LOW);
  digitalWrite(rel2, LOW);
  GTimerflash1.setTimeout(DelayFlash);
  //test.setDirection(NORM_CLOSE);
  //prog.setDirection(NORM_CLOSE);
  //mic.setDirection(NORM_CLOSE);
  mic.setTimeout(3000);   //время до старта дозаторов

}
void loop() {
if (sirop1time > 4000){
  sirop1time  = 2000;
}
if (sirop2time > 4000){
  sirop2time  = 2000;
}

  if (GTimerflash1.isReady() && a == 0 ) {
    digitalWrite(led2, HIGH);
    digitalWrite(led4, LOW);
    GTimerflash2.setTimeout(DelayFlash);
  }
  if (GTimerflash2.isReady() && a == 0) {
    digitalWrite(led4, HIGH);
    digitalWrite(led2, LOW);
    GTimerflash1.setTimeout(DelayFlash);
  }
if (Click.isReady()) {
    Click2.setTimeout(buttTimeout);
  }
  if (Click2.isReady()) {
    a = 0;
    digitalWrite(led2, HIGH);
    digitalWrite(led4, LOW);
    GTimerflash2.setTimeout(DelayFlash);
  }
  but1.tick();
  if (but1.isDouble()) {
    if (a == 3) {
      digitalWrite(rel1, HIGH);
      delay(5000);
      digitalWrite(rel1, LOW);
    }
  }
  if (a != 1  && b == 0) {
    if (but1.isPress()) {
      if (a == 3) {
        digitalWrite(rel1, HIGH);
        sirop1.setTimeout(sirop1time);
      }
      if (a == 0 || a == 2 && b == 0) {
        a = 1;
        Click.setTimeout(buttTimeout);
        digitalWrite(led2, HIGH);
        digitalWrite(led4, LOW);
      }
    }
  }
  if (a == 1 && b == 0) {
    if (but1.isPress()) {
      a = 0;
      digitalWrite(led2, LOW);
      digitalWrite(led4, HIGH);
      GTimerflash1.setTimeout(DelayFlash);
    }
  }

  if (but1.isHold()) {
    if (a == 4) {
      digitalWrite(led2, HIGH);
      digitalWrite(led4, LOW);
      digitalWrite(rel1, HIGH);
      myTimer1 = millis();
      a = 5;
    }
    if (a == 5) {
      digitalWrite(led2, HIGH);
      digitalWrite(led4, LOW);
      digitalWrite(rel1, HIGH);
    }
    if (a == 6) {
      digitalWrite(led2, HIGH);
      digitalWrite(led4, LOW);
      digitalWrite(rel1, HIGH);
      myTimer1 = millis() - sirop1time;
      a = 5;
    }
  }
  if (but1.isRelease()) {
    if (a == 5) {
      sirop1time = (millis() - myTimer1);
      EEPROM.put(4, sirop1time);
      digitalWrite(led2, LOW);
      digitalWrite(led4, LOW);
      digitalWrite(rel1, LOW);
      a = 6;
    }
  }
  but2.tick();

  if (but2.isDouble()) {
    if (a == 3) {
      digitalWrite(rel2, HIGH);
      delay(5000);
      digitalWrite(rel2, LOW);
    }
  }
  if (a != 2  && b == 0) {
    if (but2.isPress()) {
      if (a == 3) {
        digitalWrite(rel2, HIGH);
        sirop2.setTimeout(sirop2time);
      }
      if (a == 0 || a == 1 && b == 0) {
        a = 2;
        Click.setTimeout(buttTimeout);
        digitalWrite(led2, LOW);
        digitalWrite(led4, HIGH);
      }
    }
  }
  if (a == 2 && b == 0) {
    if (but2.isPress()) {
      a = 0;
      digitalWrite(led2, HIGH);
      digitalWrite(led4, LOW);
      GTimerflash1.setTimeout(DelayFlash);
    }
  }
  if (but2.isHold()) {
    if (a == 4) {
      digitalWrite(led4, HIGH);
      digitalWrite(led2, LOW);
      digitalWrite(rel2, HIGH);
      myTimer2 = millis();
      a = 5;
    }
    if (a == 5) {
      digitalWrite(led4, HIGH);
      digitalWrite(led2, LOW);
      digitalWrite(rel2, HIGH);
    }
    if (a == 6) {
      digitalWrite(led4, HIGH);
      digitalWrite(led2, LOW);
      digitalWrite(rel2, HIGH);
      myTimer2 = millis() - sirop2time;
      a = 5;
    }
  }
  if (but2.isRelease()) {
    if (a == 5) {
      sirop2time = (millis() - myTimer2);
      EEPROM.put(3, sirop2time);
      digitalWrite(led4, LOW);
      digitalWrite(led2, LOW);
      digitalWrite(rel2, LOW);
      a = 6;
    }
  }
  mic.tick();
  if (mic.isHold() && b == 0) {
    digitalWrite(led4, HIGH);
    digitalWrite(led2, HIGH);
    b = 1;
    if (a == 1) {
      digitalWrite(rel1, HIGH);
      sirop1.setTimeout(sirop1time);
      a = 0;
    }
    if (a == 2) {
      digitalWrite(rel2, HIGH);
      sirop2.setTimeout(sirop2time);
      a = 0;
    }
  }
  if (mic.isRelease() && b ==1) {
    a = 0;
    b = 0;
    digitalWrite(led2, LOW);
    digitalWrite(led4, HIGH);
    GTimerflash1.setTimeout(DelayFlash);
  }
  if (sirop1.isReady()) {
    digitalWrite(rel1, LOW);
    digitalWrite(rel2, LOW);
    a = 0;
    GTimerflash1.setTimeout(DelayFlash);
  }
  if (sirop2.isReady()) {
    digitalWrite(rel1, LOW);
    digitalWrite(rel2, LOW);
    a = 0;
     GTimerflash1.setTimeout(DelayFlash);
  }
  test.tick();
  if (test.isClick()) {
    a = 3;
    digitalWrite(led4, HIGH);
    digitalWrite(led2, HIGH);
  }
  prog.tick();
  if (prog.isClick()) {
    a = 4;
    digitalWrite(led4, LOW);
    digitalWrite(led2, LOW);
  }
  if (prog.isHold()) {
    a = 0;
    digitalWrite(led2, HIGH);
    digitalWrite(led4, LOW);
    GTimerflash2.setTimeout(DelayFlash);
  }
}
