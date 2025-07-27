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

  // Rotary draaien
  if (millis() - lastTurn > debounceDelay) {
    int currentState = digitalRead(pinA);
    if (currentState != lastState) {
      lastTurn = millis();
      bool dir = digitalRead(pinB) != currentState;

      if (colorMode) {
        // Kleur wisselen tussen wit en blauw
        uint32_t newColor = dir ? RGBW32(255, 255, 255, 0) : RGBW32(0, 0, 255, 0);
        setColor(0, newColor, 0);  // eerste 0 = segment 0, laatste 0 = no white channel
        colorUpdated(CALL_MODE_DIRECT_CHANGE);
      } else {
        // Dimmen in stappen van 15
        bri = constrain(bri + (dir ? 15 : -15), 0, 255);
        colorUpdated(CALL_MODE_DIRECT_CHANGE);
      }

      lastState = currentState;
    }
  }

  // Knop indrukken → wissel mode
  bool currentButtonState = digitalRead(pinButton);
  if (currentButtonState != lastButtonState && currentButtonState == LOW) {
    colorMode = !colorMode;
  }
  lastButtonState = currentButtonState;
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

// Zorg dat dit na alle functies komt!
static Usermod_RotaryRackDimmer rotaryMod;
REGISTER_USERMOD(rotaryMod);
