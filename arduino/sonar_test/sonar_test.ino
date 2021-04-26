#include <NewPing.h>

#define SONAR_TRIG 5
#define SONAR_ECHO 3

NewPing sonar(SONAR_TRIG, SONAR_ECHO);

void setup() {
  Serial.begin(9600);
}

void loop() {
  int d = sonar.ping_cm();
  Serial.println(d);
  delay(200);
}
