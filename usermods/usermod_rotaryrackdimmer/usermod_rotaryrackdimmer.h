#pragma once
#include "wled.h"

class Usermod_RotaryRackDimmer : public Usermod {
private:
  int pinA = 33;
  int pinB = 13;
  int pinButton = 2;

  int lastState = HIGH;
  int lastButtonState = HIGH;
  unsigned long lastTurn = 0;
  unsigned long debounceDelay = 5;

  bool colorMode = false;       // false = dim, true = color
  float colorBlend = 0.0f;      // 0.0 = blauw, 1.0 = wit
  bool initDone = false;
  bool serializeConfigOnNextTick = false;

public:
  void setup() override;
  void loop() override;
  void addToJsonInfo(JsonObject& root) override;
  uint16_t getId() override;
  void addToConfig(JsonObject& root) override;
  bool readFromConfig(JsonObject& root) override;
};
