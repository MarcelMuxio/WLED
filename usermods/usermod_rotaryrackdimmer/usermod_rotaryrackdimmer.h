#pragma once

#include "wled.h"

class Usermod_RotaryRackDimmer : public Usermod {
private:
  int pinA = 33;
  int pinB = 13;
  int pinButton = 2;

  int lastState = 0;
  bool lastButtonState = true;
  unsigned long lastTurn = 0;
  const unsigned long debounceDelay = 50;
  bool colorMode = false;
  bool initDone = false;

  float colorBlend = 0.0f;  // 0.0 = blauw, 1.0 = wit

public:
  void setup() override;
  void loop() override;
  void addToJsonInfo(JsonObject& root) override;
  uint16_t getId() override;
};
