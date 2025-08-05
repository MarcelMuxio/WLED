#pragma once
#include "wled.h"
#include <FastLED.h>

class Usermod_RotaryRackDimmer : public Usermod {
private:
  // Encoder pinconfiguratie
  int pinA = 33;
  int pinB = 13;
  int pinButton = 2;

  // Encoder status
  int lastState = HIGH;
  int lastButtonState = HIGH;
  unsigned long lastTurn = 0;
  unsigned long debounceDelay = 5;

  // Kleurblend tussen twee vaste kleuren
  float blendAmount = 0.0f; // 0.0 = color1, 1.0 = color2
  CRGB color1 = CRGB::Blue;
  CRGB color2 = CRGB::White;

  // Helderheid regelen via encoder als knop NIET is ingedrukt
  bool initDone = false;
  bool serializeConfigOnNextTick = false;

  void applyBlendedColor(); // kleur toepassen op basis van blendAmount

public:
  void setup() override;
  void loop() override;
  void addToJsonInfo(JsonObject& root) override;
  uint16_t getId() override;
  void addToConfig(JsonObject& root) override;
  bool readFromConfig(JsonObject& root) override;
};
