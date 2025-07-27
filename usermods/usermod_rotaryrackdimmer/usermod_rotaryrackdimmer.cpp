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

  // Rotary draai (dim of kleurwissel)
  if (millis() - lastTurn > debounceDelay) {
    int currentState = digitalRead(pinA);
    if (currentState != lastState) {
      lastTurn = millis();
      bool dir = digitalRead(pinB) != currentState;

if (isColorMode) {
  // Wissel tussen wit/blauw
  colorIndex = 1 - colorIndex;
  CRGB newColor = presetColors[colorIndex];
  col[0] = newColor;
    colorUpdated(CALL_MODE_DIRECT_CHANGE);
  }
      } else {
        // Dimmodus
        if (dir) {
          bri = min(255, bri + 15);
        } else {
          bri = max(0, bri - 15);
        }
        colorUpdated(CALL_MODE_DIRECT_CHANGE);
      }

      lastState = currentState;
    }
  }

  // Drukknop detecteren
  bool currentButtonState = digitalRead(pinButton);
  if (lastButtonState != currentButtonState) {
    lastButtonState = currentButtonState;
    if (!currentButtonState && (millis() - lastButtonPress > debounceDelay)) {
      isColorMode = !isColorMode;
      lastButtonPress = millis();
    }
  }
}

void Usermod_RotaryRackDimmer::addToJsonInfo(JsonObject &root) {
  JsonObject user = root["u"];
  if (!user) user = root.createNestedObject("u");

  JsonObject mod = user.createNestedObject("RotaryRackDimmer");
  mod["Brightness"] = bri;
  mod["Mode"] = isColorMode ? "Color" : "Dim";
}

#ifndef USERMOD_ID_ROTARYRACKDIMMER
#define USERMOD_ID_ROTARYRACKDIMMER 2501
#endif

uint16_t Usermod_RotaryRackDimmer::getId() {
  return USERMOD_ID_ROTARYRACKDIMMER;
}

static Usermod_RotaryRackDimmer rotaryMod;
REGISTER_USERMOD(rotaryMod);
