#pragma once

#include "wled.h"

class Usermod_RotaryRackDimmer : public Usermod {
private:
  // Pinnen van de rotary encoder (DT, CLK, SW)
  int8_t pinA = 33;
  int8_t pinB = 13;
  int8_t pinButton = 2;

  bool initDone = false;
  bool colorMode = false;
  unsigned long lastTurn = 0;
  unsigned long lastButtonPress = 0;
  int lastState = 0;
  int lastButtonState = HIGH;
  const unsigned long debounceDelay = 5;

  int fadeAmount = 5;  // stapgrootte helderheid/kleur

public:
  void setup() override {
    pinMode(pinA, INPUT_PULLUP);
    pinMode(pinB, INPUT_PULLUP);
    pinMode(pinButton, INPUT_PULLUP);
    lastState = digitalRead(pinA);
    lastButtonState = digitalRead(pinButton);
    initDone = true;
  }

  void loop() override {
    if (!initDone || strip.isUpdating()) return;

    // Rotary draai detectie
    if (millis() - lastTurn > debounceDelay) {
      int currentState = digitalRead(pinA);
      if (currentState != lastState) {
        lastTurn = millis();
        bool dir = digitalRead(pinB) != currentState;

        if (colorMode) {
          // Kleurmodus: HSV aanpassen
          CRGB fastled_col = CRGB(colPri[0], colPri[1], colPri[2]);
          CHSV hsv = rgb2hsv_approximate(fastled_col);
          int16_t newHue = hsv.h + (dir ? fadeAmount : -fadeAmount);
          if (newHue < 0) newHue += 255;
          if (newHue > 255) newHue -= 255;
          hsv.h = newHue;
          hsv2rgb_rainbow(hsv, fastled_col);
          colPri[0] = fastled_col.r;
          colPri[1] = fastled_col.g;
          colPri[2] = fastled_col.b;
        } else {
          // Dimmodus
          int16_t newBri = bri + (dir ? fadeAmount : -fadeAmount);
          bri = constrain(newBri, 0, 255);
        }

        colorUpdated(CALL_MODE_DIRECT_CHANGE);
        updateInterfaces(CALL_MODE_DIRECT_CHANGE);
        lastState = currentState;
      }
    }

    // Modus wisselen via drukknop
    int currentButtonState = digitalRead(pinButton);
    if (currentButtonState != lastButtonState && currentButtonState == LOW) {
      colorMode = !colorMode;
    }
    lastButtonState = currentButtonState;
  }

  void addToJsonInfo(JsonObject& root) override {
    JsonObject user = root["u"];
    if (!user) user = root.createNestedObject("u");

    JsonObject mod = user.createNestedObject("RotaryRackDimmer");
    mod["Mode"] = colorMode ? "Color" : "Dim";
    mod["Brightness"] = bri;
    mod["Color"] = "#" + String(colPri[0], HEX) + String(colPri[1], HEX) + String(colPri[2], HEX);
  }

  uint16_t getId() override {
    return USERMOD_ID_ROTARYRACKDIMMER;
  }
};
