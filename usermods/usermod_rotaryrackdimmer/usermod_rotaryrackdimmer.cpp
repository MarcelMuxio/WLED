#include "usermod_rotaryrackdimmer.h"

void Usermod_RotaryRackDimmer::setup() {
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);
  pinMode(pinButton, INPUT_PULLUP);

  lastState = digitalRead(pinA);
  lastButtonState = digitalRead(pinButton);

  // Toepassen opgeslagen kleur bij opstart
  byte r = colorBlend * 255;
  byte g = colorBlend * 255;
  byte b = 255 - (colorBlend * 255);
  strip.getSegment(0).colors[0] = RGBW32(r, g, b, 0);

  initDone = true;
}

void Usermod_RotaryRackDimmer::loop() {
  if (!initDone || strip.isUpdating()) return;

  // Rotary encoder draaien
  if (millis() - lastTurn > debounceDelay) {
    int currentState = digitalRead(pinA);
    if (currentState != lastState) {
      lastTurn = millis();
      bool dir = digitalRead(pinB) != currentState;

      if (colorMode) {
        // Kleur aanpassen (rechts = blauwer, links = witter)
        colorBlend = constrain(colorBlend + (dir ? -0.05f : 0.05f), 0.0f, 1.0f);
        byte r = colorBlend * 255;
        byte g = colorBlend * 255;
        byte b = 255 - (colorBlend * 255);
        strip.getSegment(0).colors[0] = RGBW32(r, g, b, 0);
        colorUpdated(CALL_MODE_DIRECT_CHANGE);
        serializeConfig(); // opslaan kleur
      } else {
        // Dimmen aanpassen (rechts = feller, links = zachter)
        bri = constrain(bri + (dir ? 15 : -15), 0, 255);
        colorUpdated(CALL_MODE_DIRECT_CHANGE);
        serializeConfig(); // opslaan helderheid
      }

      lastState = currentState;
    }
  }

  // Drukknop om te wisselen tussen dim/kleur-modus
  bool currentButtonState = digitalRead(pinButton);
  if (currentButtonState != lastButtonState && currentButtonState == LOW) {
    colorMode = !colorMode;
    serializeConfig(); // opslaan modus
  }
  lastButtonState = currentButtonState;
}

void Usermod_RotaryRackDimmer::addToJsonInfo(JsonObject& root) {
  JsonObject user = root["u"];
  if (!user) user = root.createNestedObject("u");

  JsonObject mod = user.createNestedObject("RotaryRackDimmer");
  mod["Brightness"] = bri;
  mod["Mode"] = colorMode ? "Color" : "Dim";
  mod["Blend"] = colorBlend;
}

void Usermod_RotaryRackDimmer::addToConfig(JsonObject& root) {
  JsonObject top = root.createNestedObject("RotaryRackDimmer");
  top["brightness"] = bri;
  top["colorBlend"] = colorBlend;
  top["mode"] = colorMode;
}

bool Usermod_RotaryRackDimmer::readFromConfig(JsonObject& root) {
  JsonObject top = root["RotaryRackDimmer"];
  bool configComplete = !top.isNull();

  configComplete &= getJsonValue(top["brightness"], bri);
  configComplete &= getJsonValue(top["colorBlend"], colorBlend);
  configComplete &= getJsonValue(top["mode"], colorMode);

  return configComplete;
}

#ifndef USERMOD_ID_ROTARYRACKDIMMER
#define USERMOD_ID_ROTARYRACKDIMMER 2501
#endif

uint16_t Usermod_RotaryRackDimmer::getId() {
  return USERMOD_ID_ROTARYRACKDIMMER;
}

static Usermod_RotaryRackDimmer rotaryMod;
REGISTER_USERMOD(rotaryMod);
