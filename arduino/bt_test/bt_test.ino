/***********************************************************************************
 * Script de test para el módulo bluetooth.
 * Permite enviar comandos AT desde el monitor serie.
 * IMPORTANTE: Para que funcionen bien los comandos AT, configurar el monitor Serial
 * para que incluya ámbos caracteres NL+CR como fin de línea
 ***********************************************************************************/

// Librería
#include <SoftwareSerial.h>

// Pines
#define BLUETOOTH_RX 2
#define BLUETOOTH_TX 4

// Módulo bluetooth
SoftwareSerial bluetooth(BLUETOOTH_RX, BLUETOOTH_TX); // RX, TX


void setup() {
  // Configura comunicación serie y bluetooth
  Serial.begin(9600);
  bluetooth.begin(9600);

  // Espera a que el módulo bluetooth esté en modo pairing
  Serial.println("Iniciado");
  bt_waitPairable();
  Serial.println("");
  Serial.println("PAIRABLE!!!");
  Serial.println("");
  /*
  bt_waitConnected();
  Serial.println("");
  Serial.println("CONNECTED!!!");
  Serial.println("");
  */
}

void loop() {
  // Lee caracteres del módulo bluetooth y los escribe en el monitor serie
  if (bluetooth.available())
    Serial.write(bluetooth.read());
  
  // Lee caracteres del monitor serie y los envía al módulo bluetooth 
  if (Serial.available()) 
    bluetooth.write(Serial.read());
}


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
