#pragma once

#include "wled.h"

class Usermod_RotaryRackDimmer : public Usermod {
private:
  int pinA = 33;          // Rotary encoder CLK
  int pinB = 13;          // Rotary encoder DT
  int pinButton = 12;     // Drukknop van rotary encoder
  int lastState = 0;
  bool isColorMode = false;
  bool lastButtonState = true;
  unsigned long lastButtonPress = 0;
  const unsigned long debounceDelay = 200;
  unsigned long lastTurn = 0;

  uint8_t colorIndex = 0;
  CRGB presetColors[2] = {
    CRGB(255, 0, 0), // Wit
    CRGB(0, 255, 0)      // Blauw
  };

  bool initDone = false;

public:
  void setup() override;
  void loop() override;
  void addToJsonInfo(JsonObject &root) override;
  uint16_t getId() override;
};
