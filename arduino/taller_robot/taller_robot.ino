/******************************************************************
 * Programa de control para un Arduino Smart Car Kit
 * Preparado para controlarse por Bluetooth con la app
 * Arduino Bluetooth RC Car https://play.google.com/store/apps/details?id=braulio.calle.bluetoothRCcontroller4
 * Dos modos de funcionamiento:
 *   - Control manual por Bluetooth
 *   - Modo autónomo con detección de obstáculos
 *
 * by: Jorge Guilló <jguillo@gmail.com>
 */

// Librerías
#include <SoftwareSerial.h>
#include <NewPing.h>
#include "TM1637.h"

// Definición de pines
#define BLUETOOTH_RX 2
#define BLUETOOTH_TX 4
#define SDA A4
#define SCL A5
#define SONAR_TRIG 5
#define SONAR_ECHO 3
#define MOTOR_LSPEED 9 //ENA
#define MOTOR_LFWD 8 //IN1
#define MOTOR_LREV 11 //IN2
#define MOTOR_RSPEED 10 //ENB
#define MOTOR_RFWD 13 //IN3
#define MOTOR_RREV 12 //IN4

// Velocidades en modo autónomo
#define VEL_ALTA 175
#define VEL_BAJA 125
#define VEL_GIRO 175

// Módulo bluetooth
SoftwareSerial bluetooth(BLUETOOTH_RX, BLUETOOTH_TX);
// Sensor ultrasonidos
NewPing sonar(SONAR_TRIG, SONAR_ECHO);
// Pantalla digital
TM1637 disp(SCL,SDA);

// Valor inicial de la pantalla "----"
int8_t guiones[] = { INDEX_NEGATIVE_SIGN, INDEX_NEGATIVE_SIGN, INDEX_NEGATIVE_SIGN, INDEX_NEGATIVE_SIGN };

// Indicador de modo auto
boolean modo_auto = false;
// Sentido giro en modo auto (va alternando)
boolean izq = true;
// Velocidad actual en modo manual
int velocidad = 150;

void setup() {
  // Configuración pines E/S
  pinMode(SDA, OUTPUT);
  pinMode(SCL, OUTPUT);
  pinMode(SONAR_TRIG, OUTPUT);
  pinMode(SONAR_ECHO, INPUT);
  pinMode(MOTOR_LSPEED, OUTPUT);
  pinMode(MOTOR_LFWD, OUTPUT);
  pinMode(MOTOR_LREV, OUTPUT);
  pinMode(MOTOR_RSPEED, OUTPUT);
  pinMode(MOTOR_RFWD, OUTPUT);
  pinMode(MOTOR_RREV, OUTPUT);
  
  // Inicio comunicaciones serie y Bluetooth
  Serial.begin(9600);
  bluetooth.begin(9600);
  
  // Inicialización pantalla
  disp.set(BRIGHT_TYPICAL);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  disp.init(D4056A);//D4056A is the type of the module
  
  // Mostrar guiones inicialmente
  disp.display(guiones);

  // Esperar a conexión por Bluetooth
  bt_waitPairable();
  bt_waitConnected();

  // Mostrar cuenta atrás
  for (int i=5; i>=0; i--) {
    disp.display(i);
    delay(1000);
  }
  disp.clearDisplay();
}

void loop() {
  // Si se han recibido comandos por bluetooth
  while (bluetooth.available()) {
    // Lee el comando y lo ejecuta
    char cmd = bluetooth.read();
    ejecutar_comando(cmd);
  }
  // Si estamos en modo auto, ejecutarlo
  if (modo_auto) {
    autonomo();
  }
}

// Funcionamiento en modo auto
void autonomo() {
    // Leer distancia
    int distancia = sonar.ping_cm();
    // Muestra distancia en la pantalla
    disp.display(distancia);
    if (distancia == 0) {
      // Error lectura
    }
    else if (distancia < 50) {
      // Hay un obstáculo cerca, parar
      frenar();
      // Mientras haya obstáculo
      while (distancia < 60) {
        // Girar durante 300ms
        if (izq) {
          girar_izq(VEL_GIRO);
        }
        else {
          girar_der(VEL_GIRO);
        }
        delay(300);
        // Parar
        frenar();
        // Volver a leer la distancia
        distancia = sonar.ping_cm();
      }
      // Cambiar sentido de giro para el próxima obstáculo
      izq = !izq;
      // Avanzar
      adelante(VEL_ALTA);
    }
    else {
      // Si no hay obstáculo, avanzar
      adelante(VEL_ALTA);
    }
}

// Ejecutar comandos bluetooth recibidos de la app
void ejecutar_comando(char cmd) {
  switch (cmd) {
    case 'F':
      // Adelante
      motor_izq(velocidad, true);
      motor_der(velocidad, true);
      break;
    case 'B':
      // Atrás
      motor_izq(velocidad, false);
      motor_der(velocidad, false);
      break;
    case 'L':
      // Giro izquierda
      motor_izq(velocidad, false);
      motor_der(velocidad, true);
      break;
    case 'R':
      // Giro derecha
      motor_izq(velocidad, true);
      motor_der(velocidad, false);
      break;
    case 'G':
      // Adelante izquierda
      motor_izq(0,true);
      motor_der(velocidad, true);
      break;
    case 'I':
      // Adelante derecha
      motor_izq(velocidad,true);
      motor_der(0, true);
      break;
    case 'H':
      // Atrás izquierda
      motor_izq(0, false);
      motor_der(velocidad, false);
      break;
    case 'J':
      // Atrás derecha
      motor_izq(velocidad, false);
      motor_der(0, false);
      break;
    case 'S':
      // Parar (velocidad 0)
      parar();
      break;
    case 'D':
      // Frenar (bloqueo de ruedas)
      frenar();
      break;
    case 'W':
      // Luz delantera ON
      break;
    case 'w':
      // Luz delantera OFF
      break;
    case 'U':
      // Luz trasera ON
      break;
    case 'u':
      // Luz trasera OFF
      break;
    case 'V':
      // Claxon ON
      break;
    case 'v':
      // Claxon OFF
      break;
    case 'X':
      // Modo auto ON
      modo_auto = true;
      break;
    case 'x':
      // Modo auto OFF
      frenar();
      disp.clearDisplay();
      modo_auto = false;
      break;

    // Ajuste de velocidad
    case '0':
      velocidad = 0;
      break;
    case '1':
      velocidad = 25;
      break;
    case '2':
      velocidad = 50;
      break;
    case '3':
      velocidad = 75;
      break;
    case '4':
      velocidad = 100;
      break;
    case '5':
      velocidad = 125;
      break;
    case '6':
      velocidad = 150;
      break;
    case '7':
      velocidad = 175;
      break;
    case '8':
      velocidad = 200;
      break;
    case '9':
      velocidad = 225;
      break;
    case 'q':
      velocidad = 250;
      break;
  }
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



/****** FUNCIONES BLUETOOTH **************/

/** ESTADOS BLUETOOTH **/
#define INITIALIZING 0
#define READY        1
#define INQUIRING    2
#define PAIRABLE     3
#define CONNECTING   4
#define CONNECTED    5

int bt_status = INITIALIZING;

// Esperar a que el módulo bluetooth esté en modo pairing
void bt_waitPairable(){
  while(bt_status != PAIRABLE){
    if(bluetooth.available()){
      String recvString = bt_readLine();
      if(recvString == "+PAIRABLE") {
        bt_status = PAIRABLE;
      }
    }
    else{
      if(bt_testAT()) {
         bt_status = PAIRABLE;
      }
      else{ 
	delay(200);
        if(bt_testAT()) {
          bt_status = PAIRABLE;
        }
        else{ 
          bt_status = CONNECTED;
          break;
        }
      }
    } 
  }

}

// Esperar a que haya conexión bluetooth
void bt_waitConnected(){
  while(bt_status != CONNECTED){
    if(bluetooth.available()){
      String recvString = bt_readLine();
      if(recvString.startsWith("+CONNECTING")) {
        bt_status = CONNECTING;
      }
      if(recvString == "CONNECTED") {
        bt_status = CONNECTED;
      }	
    }
    else{
      if(bt_testAT()) {
          bt_status = PAIRABLE;
      }
      else{ 
        delay(200);
        if(bt_testAT()) {
           bt_status = PAIRABLE;
        }
        else {
          bt_status = CONNECTED;
        }
      }
    } 
  }
}

// Descartar comandos bluetooth pendientes
void bt_clearBuffer(){
  char recvChar;
  while(bluetooth.available())
    recvChar = bluetooth.read();
}

// Enviar comando AT y obtener la respuesta
String bt_writeAT(String cmd){
  bluetooth.println(cmd);
  delay(500);
  if(bluetooth.available() > 1) {
    return bt_readLine();
  }
  else return "";
}

// Comprobar funcionamiento de comandos AT
bool bt_testAT(){
  bt_clearBuffer();
  String res = bt_writeAT("AT");
  return res == "OK";
}

// Leer línea de texto del bluetooth (respuesta AT)
String bt_readLine() {
  char recvChar = ' ';
  String recvString;
  while(recvChar != '\n') {
    recvChar = bluetooth.read();
    if (recvChar != '\n' && recvChar != '\r') {
      recvString += recvChar;
    }
  }
  return recvString;
}  
