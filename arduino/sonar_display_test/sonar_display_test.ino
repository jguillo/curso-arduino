#include <NewPing.h>
#include "TM1637.h"

#define SDA A4
#define SCL A5

#define SONAR_TRIG 5
#define SONAR_ECHO 3

NewPing sonar(SONAR_TRIG, SONAR_ECHO);
TM1637 disp(SCL,SDA);

void setup() {
  Serial.begin(9600);
  pinMode(SDA, OUTPUT);
  pinMode(SCL, OUTPUT);

  disp.set(BRIGHT_TYPICAL);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  disp.init(D4056A);//D4056A is the type of the module
}

void loop() {
  int d = sonar.ping_cm();
  disp.display(d);
  delay(200);
}
