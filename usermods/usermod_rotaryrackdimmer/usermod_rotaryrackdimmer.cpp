#include "usermod_rotaryrackdimmer.h"

void Usermod_RotaryRackDimmer::setup() {
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);
  pinMode(pinButton, INPUT_PULLUP);
  lastState = digitalRead(pinA);
  lastButtonState = digitalRead(pinButton);
  initDone = true;
}

void Usermod_RotaryRackDimmer::loop() {
  if (!initDone || strip.isUpdating()) return;

  // Rotary encoder draaien
  if (millis() - lastTurn > debounceDelay) {
    int currentState = digitalRead(pinA);
    if (currentState != lastState) {
      lastTurn = millis();
      bool dir = digitalRead(pinB) != currentState;

      if (colorMode) {
        // Wissel tussen wit en blauw
        CRGB newColor = dir ? CRGB::White : CRGB::Blue;
        color = newColor;
colorUpdated(CALL_MODE_DIRECT_CHANGE);
      } else {
        // Dimmen: grotere stappen (15 ipv 5)
        bri = constrain(bri + (dir ? 15 : -15), 0, 255);
        colorUpdated(CALL_MODE_DIRECT_CHANGE);
      }
      lastState = currentState;
    }
  }

  // Drukknop voor wisselen van modus
  bool currentButtonState = digitalRead(pinButton);
  if (currentButtonState != lastButtonState && currentButtonState == LOW) {
setColor(0, RGBW32(newColor.r, newColor.g, newColor.b, 0), 0);
colorUpdated(CALL_MODE_DIRECT_CHANGE);
}

void Usermod_RotaryRackDimmer::addToJsonInfo(JsonObject &root) {
  JsonObject user = root["u"];
  if (!user) user = root.createNestedObject("u");

  JsonObject mod = user.createNestedObject("RotaryRackDimmer");
  mod["Brightness"] = bri;
  mod["Mode"] = colorMode ? "Color" : "Dim";
}

#ifndef USERMOD_ID_ROTARYRACKDIMMER
#define USERMOD_ID_ROTARYRACKDIMMER 2501
#endif

uint16_t Usermod_RotaryRackDimmer::getId() {
  return USERMOD_ID_ROTARYRACKDIMMER;
}

static Usermod_RotaryRackDimmer rotaryMod;
REGISTER_USERMOD(rotaryMod);
