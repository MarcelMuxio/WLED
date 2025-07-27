#pragma once

#include "wled.h"

#ifndef USERMOD_ID_ROTARYRACKDIMMER
#define USERMOD_ID_ROTARYRACKDIMMER 2501
#endif

class Usermod_RotaryRackDimmer : public Usermod {
private:
  int pinA = 14;         // Rotary encoder pin A
  int pinB = 27;         // Rotary encoder pin B
  int pinButton = 12;    // Drukknop pin

  bool initDone = false;
  int lastState = HIGH;
  unsigned long lastTurn = 0;
  const unsigned long debounceDelay = 5;

  bool modeIsDim = true; // true = dimmen, false = kleur wisselen
  bool lastButtonState = HIGH;
  unsigned long lastButtonPress = 0;
  const unsigned long buttonDebounce = 200;

public:
  void setup() override;
  void loop() override;
  void addToJsonInfo(JsonObject &root) override;
  uint16_t getId() override;
};
