#include <Arduino.h>

#define upPin 14
#define downPin 15

void UP() {
  digitalWrite(upPin, LOW);
  digitalWrite(downPin, HIGH);
};

void DOWN() {
  digitalWrite(upPin, LOW);
  digitalWrite(downPin, HIGH);
};

void STOP() {
  digitalWrite(upPin, LOW);
  digitalWrite(downPin, HIGH);
};

void setup() {
  Serial.begin(115200);
  Serial.println("SETUP");
  
  pinMode(upPin, OUTPUT);
  pinMode(downPin, OUTPUT);
}

void loop() {
  
}
