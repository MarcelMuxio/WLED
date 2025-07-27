#pragma once

#include "wled.h"

class Usermod_RotaryRackDimmer : public Usermod {
  private:
    bool initDone = false;

    // GPIO pinnen voor rotary encoder en knop
    const uint8_t pinA = 32;       // Pas aan indien nodig
    const uint8_t pinB = 33;       // Pas aan indien nodig
    const uint8_t pinButton = 25;  // Drukknop pin

    // Voor encoder en knop
    int lastState = HIGH;
    unsigned long lastTurn = 0;
    const unsigned long debounceDelay = 50;

    bool lastButtonState = HIGH;
    bool colorMode = false;  // false = dim, true = kleurmodus

  public:
    void setup() override;
    void loop() override;
    void addToJsonInfo(JsonObject& root) override;
    uint16_t getId() override;
};
