#include <SoftwareSerial.h>
#include <NewPing.h>
#include "TM1637.h"



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

#define VEL_ALTA 160
#define VEL_BAJA 120
#define VEL_GIRO 160


SoftwareSerial bluetooth(BLUETOOTH_RX, BLUETOOTH_TX);
NewPing sonar(SONAR_TRIG, SONAR_ECHO);
TM1637 disp(SCL,SDA);

int8_t guiones[] = { INDEX_NEGATIVE_SIGN, INDEX_NEGATIVE_SIGN, INDEX_NEGATIVE_SIGN, INDEX_NEGATIVE_SIGN };

void setup() {
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
  
  Serial.begin(9600);
  bluetooth.begin(9600);
  
  disp.set(BRIGHT_TYPICAL);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  disp.init(D4056A);//D4056A is the type of the module
  
  disp.display(guiones);

  bt_waitPairable();
  bt_waitConnected();

  for (int i=5; i>=0; i--) {
    disp.display(i);
    delay(1000);
  }
  disp.clearDisplay();
}

boolean izq = true;
boolean modo_auto = false;
int velocidad = 120;

void loop() {
  while (bluetooth.available()) {
    char cmd = bluetooth.read();
    ejecutar_comando(cmd);
  }
  if (modo_auto) {
    autonomo();
  }
}

void autonomo() {
    int distancia = sonar.ping_cm();
    disp.display(distancia);
    if (distancia == 0) {
      // Error lectura
    }
    else if (distancia < 50) {
      frenar();
      while (distancia < 60) {
        if (izq) {
          girar_izq(VEL_GIRO);
        }
        else {
          girar_der(VEL_GIRO);
        }
        delay(300);
        frenar();
        distancia = sonar.ping_cm();
      }
      izq = !izq;
      adelante(VEL_ALTA);
    }
    else {
      adelante(VEL_ALTA);
    }
}

void ejecutar_comando(char cmd) {
  switch (cmd) {
    case 'F':
      motor_izq(velocidad, true);
      motor_der(velocidad, true);
      break;
    case 'B':
      motor_izq(velocidad, false);
      motor_der(velocidad, false);
      break;
    case 'L':
      motor_izq(velocidad, false);
      motor_der(velocidad, true);
      break;
    case 'R':
      motor_izq(velocidad, true);
      motor_der(velocidad, false);
      break;
    case 'G':
      motor_izq(0,true);
      motor_der(velocidad, true);
      break;
    case 'I':
      motor_izq(velocidad,true);
      motor_der(0, true);
      break;
    case 'H':
      motor_izq(0, false);
      motor_der(velocidad, false);
      break;
    case 'J':
      motor_izq(velocidad, false);
      motor_der(0, false);
      break;
    case 'S':
      parar();
      break;
    case 'D':
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
      modo_auto = true;
      break;
    case 'x':
      modo_auto = false;
      break;
    case '0':
      velocidad = 0;
      break;
    case '1':
      velocidad = 24;
      break;
    case '2':
      velocidad = 24 * 2;
      break;
    case '3':
      velocidad = 24 * 3;
      break;
    case '4':
      velocidad = 24 * 4;
      break;
    case '5':
      velocidad = 24 * 5;
      break;
    case '6':
      velocidad = 24 * 6;
      break;
    case '7':
      velocidad = 24 * 7;
      break;
    case '8':
      velocidad = 24 * 8;
      break;
    case '9':
      velocidad = 24 * 9;
      break;
    case 'q':
      velocidad = 24 * 10;
      break;
  }
}

void motor_izq(int velocidad, boolean adelante) {
  digitalWrite(MOTOR_LFWD, adelante ? HIGH : LOW);
  digitalWrite(MOTOR_LREV, adelante ? LOW : HIGH);
  analogWrite(MOTOR_LSPEED, velocidad);
}

void motor_der(int velocidad, boolean adelante) {
  digitalWrite(MOTOR_RFWD, adelante ? HIGH : LOW);
  digitalWrite(MOTOR_RREV, adelante ? LOW : HIGH);
  analogWrite(MOTOR_RSPEED, velocidad);
}

void adelante(int velocidad) {
   motor_izq(velocidad, true);
   motor_der(velocidad, true);
}

void atras(int velocidad) {
   motor_izq(velocidad, false);
   motor_der(velocidad, false);
}

void girar_izq(int velocidad) {
   motor_izq(velocidad, false);
   motor_der(velocidad, true);
}

void girar_der(int velocidad) {
   motor_izq(velocidad, true);
   motor_der(velocidad, false);
}

void parar() {
  motor_izq(0, true);
  motor_der(0, true);
}

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

void bt_clearBuffer(){
  char recvChar;
  while(bluetooth.available())
    recvChar = bluetooth.read();
}

String bt_writeAT(String cmd){
  bluetooth.println(cmd);
  delay(500);
  if(bluetooth.available() > 1) {
    return bt_readLine();
  }
  else return "";
}

bool bt_testAT(){
  bt_clearBuffer();
  String res = bt_writeAT("AT");
  return res == "OK";
}

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
