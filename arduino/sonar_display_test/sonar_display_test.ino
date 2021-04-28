/******************************************************************
 * Programa de test de módulo sonar HC-SR04 
 * con una pantalla LED de 4 dígitos TM1637
 *
 * by: Jorge Guilló <jguillo@gmail.com>
 */

// Librerías
#include <NewPing.h>
#include "TM1637.h"

// Pines
#define SDA A4
#define SCL A5
#define SONAR_TRIG 5
#define SONAR_ECHO 3

// Sensor ultrasonidos
NewPing sonar(SONAR_TRIG, SONAR_ECHO);
// Pantalla
TM1637 disp(SCL,SDA);

void setup() {
  // Inicializa monitor serie
  Serial.begin(9600);

  // Configuración pines
  pinMode(SDA, OUTPUT);
  pinMode(SCL, OUTPUT);
  pinMode(SONAR_TRIG, OUTPUT);
  pinMode(SONAR_ECHO, INPUT);

  // Inicialización pantalla
  disp.set(BRIGHT_TYPICAL);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  disp.init(D4056A);//D4056A is the type of the module
}

void loop() {
  // Leer distancia
  int d = sonar.ping_cm();
  // Mostrarla en la pantalla
  disp.display(d);
  // Espera 200 ms
  delay(200);
}
