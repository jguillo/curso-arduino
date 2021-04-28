/******************************************************************
 * Programa de test de la pantalla LED de 4 dígitos (TM1637)
 *
 * by: Jorge Guilló <jguillo@gmail.com>
 */

// Librería 
#include "TM1637.h"

// Pines
#define SDA A4
#define SCL A5

// Pantalla
TM1637 disp(SCL,SDA);

// Número que se muestra en la pantalla
int n = 0;

void setup() {
  // Configuración pines
  pinMode(SDA, OUTPUT);
  pinMode(SCL, OUTPUT);

  // Inicialización de la pantalla
  disp.set(BRIGHT_TYPICAL);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  disp.init(D4056A);//D4056A is the type of the module
  
}

void loop() {
    // Muestra el número durante 50ms
    disp.display(n);
    delay(50);
    // Incrementa el número
    n++;
    // Si hemos llegado a 10000, vuelve a 0
    if (n > 9999) {
      n = 0;
    }
}
