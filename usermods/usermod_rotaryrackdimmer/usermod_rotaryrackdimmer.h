#pragma once
#include "wled.h"

class Usermod_RotaryRackDimmer : public Usermod {
private:
  int pinA = 12;
  int pinB = 13;
  int pinButton = 14;
  int lastState = HIGH;
  bool initDone = false;
  unsigned long lastTurn = 0;
  const unsigned long debounceDelay = 50;
  bool colorMode = false;  // false = dim, true = color
  bool lastButtonState = HIGH;

public:
  void setup() override;
  void loop() override;
  void addToJsonInfo(JsonObject& root) override;
  uint16_t getId() override;
};
