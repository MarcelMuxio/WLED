#include "usermod_rotaryrackdimmer.h"

#ifndef USERMOD_ID_ROTARYRACKDIMMER
#define USERMOD_ID_ROTARYRACKDIMMER 2501
#endif

void Usermod_RotaryRackDimmer::setup() {
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);
  pinMode(pinButton, INPUT_PULLUP);
  lastState = digitalRead(pinA);
  lastButtonState = digitalRead(pinButton);

  // Beginstand: kleur blauw
  strip.getSegment(0).colors[0] = RGBW32(0, 0, 255, 0);
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

      if (colorMode) {
        // Draairichting: rechtsom meer blauw, linksom meer wit
        colorBlend = constrain(colorBlend + (dir ? -0.05f : 0.05f), 0.0f, 1.0f);
        byte r = colorBlend * 255;
        byte g = colorBlend * 255;
        byte b = 255 - (colorBlend * 255);
        strip.getSegment(0).colors[0] = RGBW32(r, g, b, 0);
        colorUpdated(CALL_MODE_DIRECT_CHANGE);
        serializeConfigOnNextTick = true;
      } else {
        // Dimmen (rechtsom = feller)
        bri = constrain(bri + (dir ? 15 : -15), 0, 255);
        colorUpdated(CALL_MODE_DIRECT_CHANGE);
        serializeConfigOnNextTick = true;
      }

      lastState = currentState;
    }
  }

  // Modus wisselen
  bool currentButtonState = digitalRead(pinButton);
  if (currentButtonState != lastButtonState && currentButtonState == LOW) {
    colorMode = !colorMode;
    serializeConfigOnNextTick = true;
  }
  lastButtonState = currentButtonState;

  // Config opslaan als nodig
  if (serializeConfigOnNextTick) {
DynamicJsonDocument doc(1024);
JsonObject cfg = doc.to<JsonObject>();
    addToConfig(cfg);
    serializeConfig(cfg);
    serializeConfigOnNextTick = false;
  }
}

void Usermod_RotaryRackDimmer::addToJsonInfo(JsonObject& root) {
  JsonObject user = root["u"];
  if (!user) user = root.createNestedObject("u");

  JsonObject mod = user.createNestedObject("RotaryRackDimmer");
  mod["Brightness"] = bri;
  mod["Mode"] = colorMode ? "Color" : "Dim";
  mod["Blend"] = colorBlend;
}

uint16_t Usermod_RotaryRackDimmer::getId() {
  return USERMOD_ID_ROTARYRACKDIMMER;
}

// Config uitlezen bij opstarten
bool Usermod_RotaryRackDimmer::readFromConfig(JsonObject& root) {
  JsonObject top = root[F("RotaryRackDimmer")];
  bool configComplete = !top.isNull();

  configComplete &= getJsonValue(top["brightness"], bri);
  configComplete &= getJsonValue(top["blend"], colorBlend);
  configComplete &= getJsonValue(top["mode"], colorMode);

  return configComplete;
}

// Config opslaan
void Usermod_RotaryRackDimmer::addToConfig(JsonObject& root) {
  JsonObject top = root.createNestedObject(F("RotaryRackDimmer"));
  top["brightness"] = bri;
  top["blend"] = colorBlend;
  top["mode"] = colorMode;
}

// Registratie bij WLED
static Usermod_RotaryRackDimmer rotaryMod;
REGISTER_USERMOD(rotaryMod);
