/******************************************************************
 * Programa de test de módulo sonar HC-SR04 
 *
 * by: Jorge Guilló <jguillo@gmail.com>
 */
 
// Librerías
#include <NewPing.h>

// Pines
#define SONAR_TRIG 5
#define SONAR_ECHO 3

// Sensor ultrasonidos
NewPing sonar(SONAR_TRIG, SONAR_ECHO);

void setup() {
  // Inicializa monitor serie
  Serial.begin(9600);

  // Configuración pines
  pinMode(SONAR_TRIG, OUTPUT);
  pinMode(SONAR_ECHO, INPUT);

}

void loop() {
  // Leer distancia
  int d = sonar.ping_cm();
  // Mostrarla en el monitor serie
  Serial.println(d);
  // Espera 200 ms
  delay(200);
}
