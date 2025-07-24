#include "usermod_rotaryrackdimmer.h"

void Usermod_RotaryRackDimmer::setup() {
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);
  lastState = digitalRead(pinA);
}

void Usermod_RotaryRackDimmer::loop() {
  if (millis() - lastTurn < debounceDelay) return;
  int currentState = digitalRead(pinA);
  if (currentState != lastState) {
    lastTurn = millis();
    if (digitalRead(pinB) != currentState) {
      bri = min(255, bri + 5);
    } else {
      bri = max(0, bri - 5);
    }
    lastState = currentState;
    colorUpdated(CALL_MODE_DIRECT_CHANGE);
  }
}

void Usermod_RotaryRackDimmer::addToJsonInfo(JsonObject &root) {
  JsonObject user = root["u"];
  if (!user) user = root.createNestedObject("u");
  JsonObject mod = user.createNestedObject("RotaryRackDimmer");
  mod["Brightness"] = bri;
}
