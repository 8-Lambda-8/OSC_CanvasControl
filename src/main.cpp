#include <Arduino.h>
#include <WebServer_WT32_ETH01.h>

#include <OSCMessage.h>
#include <OSCBoards.h>

WiFiUDP Udp;
const unsigned int localPort = 8888;
IPAddress remoteIp;

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

  while (!Serial);

  Serial.print("\nStarting UDPSendReceive on " + String(ARDUINO_BOARD));
  Serial.println(" with " + String(SHIELD_TYPE));
  Serial.println(WEBSERVER_WT32_ETH01_VERSION);

  WT32_ETH01_onEvent();
  ETH.begin(ETH_PHY_ADDR, ETH_PHY_POWER);
  WT32_ETH01_waitForConnect();

  Serial.println(F("\nStarting connection to server..."));
  Udp.begin(localPort);

  Serial.print(F("Listening on port "));
  Serial.println(localPort);

  Serial.println("END SETUP");

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

void move(OSCMessage& msg, int addrOffset) {
  addrOffset++;
  const char* addr = (msg.getAddress() + addrOffset);
  Serial.println();
  Serial.print(addr);
  Serial.print(" ");
  if (!msg.isInt(0)) return;
  Serial.println(msg.getInt(0));

  if (addr[0] == 't' && addr[1] == 'o') {
    goTo(msg.getInt(0) * 1000);
  } else if (addr[0] == 'b' && addr[1] == 'y') {
    goBy(msg.getInt(0) * 1000);
  }
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

  OSCMessage msg;
  int size = Udp.parsePacket();
  if (size) {
    Serial.print(F("Received packet of size "));
    Serial.println(size);
    Serial.print(F("From "));
    remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(F(", port "));
    Serial.println(Udp.remotePort());

    while (size--) msg.fill(Udp.read());

    if (!msg.hasError()) {
      msg.route("/CanvasControl/move", move);
    } else {
      Serial.println("OSC msg error");
    }
  }
}
