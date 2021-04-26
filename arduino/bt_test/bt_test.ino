#include <SoftwareSerial.h>

/***********************************************************************************
 * IMPORTANTE: Para que funcionen bien los comandos AT, configurar el monitor Serial
 * paara que incluya ámbos caracteres NL+CR como fin de línea
 ***********************************************************************************/
#define BLUETOOTH_RX 2
#define BLUETOOTH_TX 4

SoftwareSerial bluetooth(BLUETOOTH_RX, BLUETOOTH_TX); // RX, TX

#define INITIALIZING 0
#define READY        1
#define INQUIRING    2
#define PAIRABLE     3
#define CONNECTING   4
#define CONNECTED    5

int bt_status = INITIALIZING;

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
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
  if (bluetooth.available())
    Serial.write(bluetooth.read());
  if (Serial.available()) 
    bluetooth.write(Serial.read());
}

void bt_waitPairable(){
  char recvChar;
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
  char recvChar;
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
  bluetooth.print(cmd);
  bluetooth.print("\r\n");
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
