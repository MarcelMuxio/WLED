#include "usermod_rotaryrackdimmer.h"

void Usermod_RotaryRackDimmer::setup() {
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);
  pinMode(pinButton, INPUT_PULLUP);
  lastState = digitalRead(pinA);
  lastButtonState = digitalRead(pinButton);
  initDone = true;
}

void Usermod_RotaryRackDimmer::loop() {
  if (!initDone || strip.isUpdating()) return;

  if (millis() - lastTurn > debounceDelay) {
    int currentState = digitalRead(pinA);
    if (currentState != lastState) {
      lastTurn = millis();
      bool dir = digitalRead(pinB) != currentState;
      WS2812FX::Segment& seg = strip.getSegment(0);
      if (colorMode) {
        // Wissel tussen wit en blauw
        seg.colors[0] = dir ? 0xFFFFFF : 0x0000FF;
      } else {
        // Dimmen via bri (die WLED verwerkt) of pseudokleur aanpassen
        bri = constrain(bri + (dir ? 15 : -15), 0, 255);
        // seg.colors[0] = bepaal op basis van kleur + brightness
      }
      lastState = currentState;
      colorUpdated(CALL_MODE_DIRECT_CHANGE);
    }
  }

  bool currentButtonState = digitalRead(pinButton);
  if (currentButtonState != lastButtonState && currentButtonState == LOW) {
    colorMode = !colorMode;
  }
  lastButtonState = currentButtonState;
}

void Usermod_RotaryRackDimmer::addToJsonInfo(JsonObject &root) {
  JsonObject user = root["u"];
  if (user.isNull()) user = root.createNestedObject("u");
  JsonObject mod = user.createNestedObject("RotaryRackDimmer");
  mod["Brightness"] = bri;
  mod["Mode"] = colorMode ? "Color" : "Dim";
}

uint16_t Usermod_RotaryRackDimmer::getId() {
  return 2501;
}

static Usermod_RotaryRackDimmer rotaryMod;
REGISTER_USERMOD(rotaryMod);
