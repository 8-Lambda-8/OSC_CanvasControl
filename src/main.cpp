#include <Arduino.h>

#define upPin 14
#define downPin 15

uint8_t state = 0;
/**
 * @brief
 * 0 Stop
 * 1 UP
 * 2 DOWN
 */

uint32_t position = 0;
uint32_t targetPosition = 0;

void UP() {
  state = 1;
  digitalWrite(upPin, LOW);
  digitalWrite(downPin, HIGH);
};

void DOWN() {
  state = 2;
  digitalWrite(upPin, HIGH);
  digitalWrite(downPin, LOW);
};

void STOP() {
  state = 0;
  digitalWrite(upPin, HIGH);
  digitalWrite(downPin, HIGH);
};

void setup() {
  Serial.begin(115200);
  Serial.println("SETUP");

  pinMode(upPin, OUTPUT);
  pinMode(downPin, OUTPUT);

  // move UP
  UP();
  delay(20 * 1000);
  STOP();
}

void goBy(int32_t distance) {
  targetPosition = position + distance;
  if (position > targetPosition) UP();
  if (position < targetPosition) DOWN();
}

void goTo(uint32_t pos) {
  targetPosition = pos;
  if (position > targetPosition) UP();
  if (position < targetPosition) DOWN();
}

uint32_t lastMillis = 0;
void loop() {
  switch (state) {
    case 1:  // UP
      position -= millis() - lastMillis;
      if (position < targetPosition) STOP();
      break;
    case 2:  // DOWN
      position += millis() - lastMillis;
      if (position > targetPosition) STOP();
      break;
    default:
      break;
  }
  lastMillis = millis();
}
