#include "TM1637.h"

#define SDA A4
#define SCL A5

TM1637 disp(SCL,SDA);

int n = 0;

void setup() {
  pinMode(SDA, OUTPUT);
  pinMode(SCL, OUTPUT);

  disp.set(BRIGHT_TYPICAL);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  disp.init(D4056A);//D4056A is the type of the module
  
}

void loop() {
    disp.display(n);
    delay(50);
    n++;
    if (n > 9999) {
      n = 0;
    }
}
