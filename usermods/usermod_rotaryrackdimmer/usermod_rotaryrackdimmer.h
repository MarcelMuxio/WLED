#pragma once

#include "wled.h"

class Usermod_RotaryRackDimmer : public Usermod {
private:
  int pinA = 33;  // A (CLK) pin van de encoder
  int pinB = 13;  // B (DT) pin van de encoder
  int lastState = 0;
  unsigned long lastTurn = 0;
  const unsigned long debounceDelay = 5; // ms

public:
  void setup() override;
  void loop() override;
  void addToConfig(JsonObject &root) override {}
  bool readFromConfig(JsonObject &root) override { return true; }
  void addToJsonInfo(JsonObject &root) override;
  String getId() override { return F("RotaryRackDimmer"); }
};
