# Taller de introducción a la robótica

Este repositorio contiene archivos de apoyo para un taller de robótica con Arduino.

En el taller se monta y se programa un kit Smart Car de Arduino, que se controla por bluetooth con la app [Arduino Bluetooth RC Car](https://play.google.com/store/apps/details?id=braulio.calle.bluetoothRCcontroller) de Android. 

- La carpeta **manuales** contiene los manuales del kit en PDF.
- La carpeta **arduino** contiene los scripts y librerías necesarios para programar el kit
  - *bt_test*: script de test del módulo bluetooth
  - *display_test*: script de test de la pantalla LED de 7 segmentos
  - *motor_test*: script de test de movimiento del Smart Car
  - *sonar_test*: script de test del módulo sensor de distancia por ultrasonidos.  
    Muestra la distancia en el monitor serie.
  - *sonar_display_test*: script de test del módulo sensor de distancia por ultrasonidos.  
    Muestra la distancia en la pantalla LED.
  - *taller_robot*: script con el programa completo
  - *libraries*: Librerías utilizadas por los scripts
    - DigitalTube: Librería de control de displays de 7 segmentos TM1637 (by Fred.Chu)
    - NewPing: Librería de control de sensores de ultrasonidos HC-SR04 (by [Tim Eckel](https://bitbucket.org/teckel12/arduino-new-ping))
