#include "usermod_rotaryrackdimmer.h"

void Usermod_RotaryRackDimmer::setup() {
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);
  pinMode(pinButton, INPUT_PULLUP);
  lastState = digitalRead(pinA);
  lastButtonState = digitalRead(pinButton);

  // Startkleur blauw
  col[0] = RGBW32(0, 0, 255, 0);
  colorBlend = 0.0f;

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
        // Langzaam overgaan tussen blauw (rechts) en wit (links)
        colorBlend = constrain(colorBlend + (dir ? -0.05f : 0.05f), 0.0f, 1.0f);
        byte r = colorBlend * 255;
        byte g = colorBlend * 255;
        byte b = 255 - (colorBlend * 255);
        col[0] = RGBW32(r, g, b, 0);
        colorUpdated(CALL_MODE_DIRECT_CHANGE);
      } else {
        // Dimmen (rechtsom = feller)
        bri = constrain(bri + (dir ? 15 : -15), 0, 255);
        colorUpdated(CALL_MODE_DIRECT_CHANGE);
      }

      lastState = currentState;
    }
  }

  // Drukknop voor modus wisselen
  bool currentButtonState = digitalRead(pinButton);
  if (currentButtonState != lastButtonState && currentButtonState == LOW) {
    colorMode = !colorMode;
  }
  lastButtonState = currentButtonState;
}

void Usermod_RotaryRackDimmer::addToJsonInfo(JsonObject& root) {
  JsonObject user = root["u"];
  if (!user) user = root.createNestedObject("u");

  JsonObject mod = user.createNestedObject("RotaryRackDimmer");
  mod["Brightness"] = bri;
  mod["Mode"] = colorMode ? "Color" : "Dim";
  mod["Blend"] = colorBlend;
}

#ifndef USERMOD_ID_ROTARYRACKDIMMER
#define USERMOD_ID_ROTARYRACKDIMMER 2501
#endif

uint16_t Usermod_RotaryRackDimmer::getId() {
  return USERMOD_ID_ROTARYRACKDIMMER;
}

static Usermod_RotaryRackDimmer rotaryMod;
REGISTER_USERMOD(rotaryMod);
