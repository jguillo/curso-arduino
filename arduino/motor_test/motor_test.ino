/******************************************************************
 * Programa de test de movimiento para un Arduino Smart Car Kit
 *
 * by: Jorge Guilló <jguillo@gmail.com>
 */

// Librerías
#include "TM1637.h"

// Pines
#define SDA A4
#define SCL A5
#define MOTOR_LSPEED 9 //ENA
#define MOTOR_LFWD 8 //IN1
#define MOTOR_LREV 11 //IN2
#define MOTOR_RSPEED 10 //ENB
#define MOTOR_RFWD 13 //IN3
#define MOTOR_RREV 12 //IN4

// Pantalla
TM1637 disp(SCL,SDA);

void setup() {
  // Configuración pines E/S
  pinMode(SDA, OUTPUT);
  pinMode(SCL, OUTPUT);
  pinMode(MOTOR_LSPEED, OUTPUT);
  pinMode(MOTOR_LFWD, OUTPUT);
  pinMode(MOTOR_LREV, OUTPUT);
  pinMode(MOTOR_RSPEED, OUTPUT);
  pinMode(MOTOR_RFWD, OUTPUT);
  pinMode(MOTOR_RREV, OUTPUT);  

  // Inicialización pantalla
  disp.set(BRIGHT_TYPICAL);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  disp.init(D4056A);//D4056A is the type of the module
  
  // Muestra cuenta atrás
  for (int i=5; i>=0; i--) {
    disp.display(i);
    delay(1000);
  }
  disp.clearDisplay();
}

int velocidad = 120;

void loop() {
  // Avanza 1 seg
  adelante(velocidad);
  delay(1000);
  frenar();
  // Retrocede 1 seg
  atras(velocidad);
  delay(1000);
  frenar();
  // Giro izquierda 1 seg
  girar_izq(velocidad);
  delay(1000);
  frenar();
  // Giro derecha 1 seg
  girar_der(velocidad);
  delay(1000);
  frenar();
  // Espera 2 seg
  delay(2000);
}

// Mover el motor izquierdo
void motor_izq(int velocidad, boolean adelante) {
  digitalWrite(MOTOR_LFWD, adelante ? HIGH : LOW);
  digitalWrite(MOTOR_LREV, adelante ? LOW : HIGH);
  analogWrite(MOTOR_LSPEED, velocidad);
}

// Mover el motor derecho
void motor_der(int velocidad, boolean adelante) {
  digitalWrite(MOTOR_RFWD, adelante ? HIGH : LOW);
  digitalWrite(MOTOR_RREV, adelante ? LOW : HIGH);
  analogWrite(MOTOR_RSPEED, velocidad);
}

// Avanzar
void adelante(int velocidad) {
   motor_izq(velocidad, true);
   motor_der(velocidad, true);
}

// Retroceder
void atras(int velocidad) {
   motor_izq(velocidad, false);
   motor_der(velocidad, false);
}

// Girar a la izquierda
void girar_izq(int velocidad) {
   motor_izq(velocidad, false);
   motor_der(velocidad, true);
}

// Girar a la derecha
void girar_der(int velocidad) {
   motor_izq(velocidad, true);
   motor_der(velocidad, false);
}

// Parar (velocidad 0)
void parar() {
  motor_izq(0, true);
  motor_der(0, true);
}

// Frenar (bloqueo de ruedas)
void frenar() {
  digitalWrite(MOTOR_LFWD, 0);
  digitalWrite(MOTOR_LREV, 0);
  digitalWrite(MOTOR_RFWD, 0);
  digitalWrite(MOTOR_RREV, 0);
}
