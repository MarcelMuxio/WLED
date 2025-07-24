#pragma once
#include "wled.h"

class Usermod_RotaryRackDimmer : public Usermod {
  private:
    int pinA = 33;
    int pinB = 13;
    int lastState = 0;
    unsigned long lastTurn = 0;
    const unsigned long debounceDelay = 50;
    bool initDone = false;

  public:
    void setup() override;
    void loop() override;
    void addToJsonInfo(JsonObject &root) override;
    uint16_t getId() override;
};
