#include "usermod_rotaryrackdimmer.h"

#ifndef USERMOD_ID_ROTARYRACKDIMMER
#define USERMOD_ID_ROTARYRACKDIMMER 2501
#endif

void Usermod_RotaryRackDimmer::applyBlendedColor() {
  CRGB blended = blend(color1, color2, uint8_t(blendAmount * 255));
  strip.getSegment(0).setColor(0, RGBW32(blended.r, blended.g, blended.b, 0));
  colorUpdated(CALL_MODE_DIRECT_CHANGE);
}

void Usermod_RotaryRackDimmer::setup() {
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);
  pinMode(pinButton, INPUT_PULLUP);
  lastState = digitalRead(pinA);
  lastButtonState = digitalRead(pinButton);

  // Start met opgeslagen blendwaarde
  applyBlendedColor();

  // Helderheid toepassen zoals opgeslagen
  bri = constrain(bri, 0, 255);
  colorUpdated(CALL_MODE_INIT);

  initDone = true;
}

void Usermod_RotaryRackDimmer::loop() {
  if (!initDone || strip.isUpdating()) return;

  if (millis() - lastTurn > debounceDelay) {
    int currentState = digitalRead(pinA);
    if (currentState != lastState) {
      lastTurn = millis();
      bool dir = digitalRead(pinB) != currentState;

      // Encoder logica: knop NIET ingedrukt → kleurblend
      if (digitalRead(pinButton) == HIGH) {
        blendAmount = constrain(blendAmount + (dir ? -0.02f : 0.02f), 0.0f, 1.0f);
        applyBlendedColor();
      } else {
        // Knop ingedrukt → dimmerfunctie
        bri = constrain(bri + (dir ? 10 : -10), 0, 255);
        colorUpdated(CALL_MODE_DIRECT_CHANGE);
      }

      serializeConfigOnNextTick = true;
      lastState = currentState;
    }
  }

  // Opslaan als nodig
  if (serializeConfigOnNextTick) {
    DynamicJsonDocument doc(1024);
    JsonObject cfg
