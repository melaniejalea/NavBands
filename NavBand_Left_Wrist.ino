#include "BluetoothSerial.h"

String device_name = "NavBand-Left-Wrist";

BluetoothSerial SerialBT;
int received;
char receivedChar;

// Caracteres utilizados para controlar el brazalete
const char turnON1 = 'a';
const char turnON2 = 'b';
const char turnON3 = 'c';
const char turnOFF = 'x';

// Pin utilizado para controlar el LED y el motor vibrador
const int LEDMotorPin = 23; 

// Variables de tiempo y parpadeo del LED
unsigned long previousMillis = 0;
const unsigned long blinkInterval = 800; // Intervalo de parpadeo 1
const unsigned long blinkInterval2 = 400; // Intervalo de parpadeo 2 
const unsigned long autoTurnOffDelay = 4000; // Tiempo para apagado automático después de enviar señal TurnON

bool ledState = false;        // Estado actual del LED
bool constantOn = false;      // Estado de encendido constante
bool autoTurnOff = false;     // Bandera de apagado automático para señales TurnON
unsigned long interval = 0;   // Intervalo de tiempo actual
unsigned long autoTurnOffTime = 0; // Tiempo para apagado automático

void setup() {
  Serial.begin(115200);
  SerialBT.begin(device_name);
  
  // Mensajes de inicio y descripción del control
  Serial.println("El brazalete NavBands se ha iniciado, se puede emparejar con Bluetooth!");
  Serial.println("Para prenderse, envie una señal de TurnON");
  Serial.println("Para abortar el ejecutamiento, envie la señal TurnOFF");
  
  pinMode(LEDMotorPin, OUTPUT); // Configurar el pin del LED y motor como salida
}

void loop() {
  if (SerialBT.available()) {
    receivedChar = (char)SerialBT.read();
    SerialBT.print("Señal recibida : ");
    SerialBT.println(receivedChar);
    Serial.print("Señal recibida : ");
    Serial.println(receivedChar);

    if (receivedChar == turnON1) {
      // Encender el LED con un intervalo de parpadeo de 0.8 segundos
      SerialBT.println("Distancia de viraje a 400-200m, favor de ir tomando el carril correspondiente");
      Serial.println("Distancia de viraje a 400-200m, favor de ir tomando el carril correspondiente");
      ledState = true;
      constantOn = false;
      autoTurnOff = true;
      interval = blinkInterval;
      previousMillis = millis();
      autoTurnOffTime = previousMillis + autoTurnOffDelay;
    }

    if (receivedChar == turnON2) {
      // Encender el LED con un intervalo de parpadeo de 0.4 segundos
      SerialBT.println("Distancia de viraje a 200-50m, prepararse para el viraje");
      Serial.println("Distancia de viraje a 200-50m, prepararse para el viraje");
      ledState = true;
      constantOn = false;
      autoTurnOff = true;
      interval = blinkInterval2;
      previousMillis = millis();
      autoTurnOffTime = previousMillis + autoTurnOffDelay;
    }

    if (receivedChar == turnON3) {
      // Encender el LED de forma constante (sin parpadeo)
      SerialBT.println("Viraje alcanzado, realice la acción con precaución");
      Serial.println("Viraje alcanzado, realice la acción con precaución");
      ledState = true;
      constantOn = true;
      autoTurnOff = true;
      digitalWrite(LEDMotorPin, HIGH);
      previousMillis = millis();
      autoTurnOffTime = previousMillis + autoTurnOffDelay;
    }

    if (receivedChar == turnOFF) {
      // Apagar el LED
      SerialBT.println("Acción cancelada");
      Serial.println("Acción cancelada");
      ledState = false;
      constantOn = false;
      autoTurnOff = false;
      digitalWrite(LEDMotorPin, LOW);
    }
  }

  unsigned long currentMillis = millis();

  if (ledState && !constantOn && currentMillis - previousMillis >= interval) {
    // Parpadeo del LED
    previousMillis = currentMillis;
    digitalWrite(LEDMotorPin, !digitalRead(LEDMotorPin));
  }

  if (autoTurnOff && currentMillis >= autoTurnOffTime) {
    // Apagado automático del LED
    SerialBT.println("Apagado automático");
    Serial.println("Apagado automático");
    ledState = false;
    autoTurnOff = false;
    digitalWrite(LEDMotorPin, LOW);
  }

  delay(20);
}
