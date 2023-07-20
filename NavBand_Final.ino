#include "BluetoothSerial.h"

String device_name = "NavBand-Simulation";

BluetoothSerial SerialBT;
int received;
char receivedChar;

// Caracteres utilizados para controlar el brazalete
const char turnON1 = 'a';
const char turnON2 = 'b';
const char turnON3 = 'c';
const char turnON4 = 'm'; // Nuevo comando
const char turnON5 = 'n'; // Nuevo comando
const char turnON6 = 'o'; // Nuevo comando
const char turnOFF = 'x';

// Pines utilizados para controlar los LED y motores vibradores
const int LEDMotorPin = 23;  // LED y motor original
const int LEDMotorPin2 = 22; // Nuevo LED y motor

// Variables de tiempo y parpadeo de los LED
unsigned long previousMillis = 0;
const unsigned long blinkInterval = 800; // Intervalo de parpadeo 1
const unsigned long blinkInterval2 = 400; // Intervalo de parpadeo 2
const unsigned long autoTurnOffDelay = 4000; // Tiempo para apagado automático después de enviar señal TurnON

// Estados de los LED
bool ledStatePin1 = false;   // Estado actual del LED del LEDMotorPin
bool ledStatePin2 = false;   // Estado actual del LED del LEDMotorPin2
bool constantOn = false;     // Estado de encendido constante
bool autoTurnOff = false;    // Bandera de apagado automático para señales TurnON
unsigned long interval = 0;  // Intervalo de tiempo actual
unsigned long autoTurnOffTime = 0; // Tiempo para apagado automático

void setup() {
  Serial.begin(115200);
  SerialBT.begin(device_name);

  // Mensajes de inicio y descripción del control
  Serial.println("El brazalete NavBands se ha iniciado, se puede emparejar con Bluetooth!");
  Serial.println("Para prenderse, envía una señal de TurnON");
  Serial.println("Para abortar el ejecutamiento, envía la señal TurnOFF");

  pinMode(LEDMotorPin, OUTPUT);  // Configurar el pin del LED y motor como salida
  pinMode(LEDMotorPin2, OUTPUT); // Configurar el nuevo pin del LED y motor como salida
}

void loop() {
  if (SerialBT.available()) {
    receivedChar = (char)SerialBT.read();
    SerialBT.print("Señal recibida: ");
    SerialBT.println(receivedChar);
    Serial.print("Señal recibida: ");
    Serial.println(receivedChar);

    if (receivedChar == turnON1) {
      SerialBT.println("Distancia de viraje a 400-200m, favor de ir tomando el carril correspondiente");
      Serial.println("Distancia de viraje a 400-200m, favor de ir tomando el carril correspondiente");
      ledStatePin1 = true;
      ledStatePin2 = false; // Asegurarse de apagar el otro LED
      constantOn = false;
      autoTurnOff = true;
      interval = blinkInterval;
      previousMillis = millis();
      autoTurnOffTime = previousMillis + autoTurnOffDelay;
    }


    if (receivedChar == turnON4) {
      SerialBT.println("Distancia de viraje a 400-200m, favor de ir tomando el carril correspondiente");
      Serial.println("Distancia de viraje a 400-200m, favor de ir tomando el carril correspondiente");
      ledStatePin1 = false;
      ledStatePin2 = true; // Asegurarse de apagar el otro LED
      constantOn = false;
      autoTurnOff = true;
      interval = blinkInterval;
      previousMillis = millis();
      autoTurnOffTime = previousMillis + autoTurnOffDelay;
    }

    if (receivedChar == turnON2) {
      SerialBT.println("Distancia de viraje a 200-50m, prepararse para el viraje");
      Serial.println("Distancia de viraje a 200-50m, prepararse para el viraje");
      ledStatePin1 = true; // Asegurarse de apagar el otro LED
      ledStatePin2 = false;
      constantOn = false;
      autoTurnOff = true;
      interval = blinkInterval2;
      previousMillis = millis();
      autoTurnOffTime = previousMillis + autoTurnOffDelay;
    }

    if (receivedChar == turnON5) {
      SerialBT.println("Distancia de viraje a 200-50m, prepararse para el viraje");
      Serial.println("Distancia de viraje a 200-50m, prepararse para el viraje");
      ledStatePin1 = false; // Asegurarse de apagar el otro LED
      ledStatePin2 = true;
      constantOn = false;
      autoTurnOff = true;
      interval = blinkInterval2;
      previousMillis = millis();
      autoTurnOffTime = previousMillis + autoTurnOffDelay;
    }


    if (receivedChar == turnON3) {
      SerialBT.println("Viraje alcanzado, realice la acción con precaución");
      Serial.println("Viraje alcanzado, realice la acción con precaución");
      ledStatePin1 = true;
      ledStatePin2 = false; // Asegurarse de apagar el otro LED
      constantOn = true;
      autoTurnOff = true;
      digitalWrite(LEDMotorPin, HIGH);
      digitalWrite(LEDMotorPin2, LOW); // Asegurarse de apagar el otro LED
      previousMillis = millis();
      autoTurnOffTime = previousMillis + autoTurnOffDelay;
    }

    if (receivedChar == turnON6) {
      SerialBT.println("Viraje alcanzado, realice la acción con precaución");
      Serial.println("Viraje alcanzado, realice la acción con precaución");
      ledStatePin1 = false;
      ledStatePin2 = true; // Asegurarse de apagar el otro LED
      constantOn = true;
      autoTurnOff = true;
      digitalWrite(LEDMotorPin, LOW);
      digitalWrite(LEDMotorPin2, HIGH); // Asegurarse de apagar el otro LED
      previousMillis = millis();
      autoTurnOffTime = previousMillis + autoTurnOffDelay;
    }

    if (receivedChar == turnOFF) {
      // Apagar ambos LED
      SerialBT.println(".");
      Serial.println(".");
      ledStatePin1 = false;
      ledStatePin2 = false;
      constantOn = false;
      autoTurnOff = false;
      digitalWrite(LEDMotorPin, LOW);
      digitalWrite(LEDMotorPin2, LOW);
    }
  }

  unsigned long currentMillis = millis();

  if (ledStatePin1 && !constantOn && currentMillis - previousMillis >= interval) {
    // Parpadeo del LED del LEDMotorPin
    previousMillis = currentMillis;
    digitalWrite(LEDMotorPin, !digitalRead(LEDMotorPin));
  }

  if (ledStatePin2 && !constantOn && currentMillis - previousMillis >= interval) {
    // Parpadeo del LED del LEDMotorPin2
    previousMillis = currentMillis;
    digitalWrite(LEDMotorPin2, !digitalRead(LEDMotorPin2));
  }

  if (autoTurnOff && currentMillis >= autoTurnOffTime) {
    // Apagado automático de ambos LED
    SerialBT.println("Apagado automático");
    Serial.println("Apagado automático");
    ledStatePin1 = false;
    ledStatePin2 = false;
    autoTurnOff = false;
    digitalWrite(LEDMotorPin, LOW);
    digitalWrite(LEDMotorPin2, LOW);
  }

  delay(20);
}
