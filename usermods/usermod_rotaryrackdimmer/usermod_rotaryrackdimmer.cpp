#include "usermod_rotaryrackdimmer.h"
#include "wled.h"

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

  // === Encoder draaien ===
  if (millis() - lastTurn > debounceDelay) {
    int currentState = digitalRead(pinA);
    if (currentState != lastState) {
      lastTurn = millis();
      bool dir = digitalRead(pinB) != currentState;

      if (modeIsDim) {
        // Dim modus
        if (dir) {
          bri = min(255, bri + 15);
        } else {
          bri = max(0, bri - 15);
        }
      } else {
        // Kleurwissel modus
        CRGB newColor = dir ? CRGB::White : CRGB::Blue;
        col[0] = newColor;
      }

      colorUpdated(CALL_MODE_DIRECT_CHANGE);
      lastState = currentState;
    }
  }

  // === Knop indrukken ===
  bool currentButtonState = digitalRead(pinButton);
  if (currentButtonState != lastButtonState && millis() - lastButtonPress > buttonDebounce) {
    lastButtonPress = millis();
    if (currentButtonState == LOW) {
      modeIsDim = !modeIsDim;
    }
    lastButtonState = currentButtonState;
  }
}

void Usermod_RotaryRackDimmer::addToJsonInfo(JsonObject &root) {
  JsonObject user = root["u"];
  if (!user) user = root.createNestedObject("u");

  JsonObject mod = user.createNestedObject("RotaryRackDimmer");
  mod["Brightness"] = bri;
  mod["Mode"] = modeIsDim ? "Dimming" : "Color Switch";
}

uint16_t Usermod_RotaryRackDimmer::getId() {
  return USERMOD_ID_ROTARYRACKDIMMER;
}

static Usermod_RotaryRackDimmer rotaryMod;
REGISTER_USERMOD(rotaryMod);
