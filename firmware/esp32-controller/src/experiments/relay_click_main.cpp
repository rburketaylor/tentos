#include <Arduino.h>

namespace {
constexpr int kRelayPin = 18;
constexpr int kRelayActiveLevel = LOW;
constexpr int kRelayIdleLevel = HIGH;
constexpr unsigned long kToggleIntervalMs = 1000;

bool relayEnabled = false;
unsigned long lastToggleAt = 0;

void setRelayState(bool enabled) {
  relayEnabled = enabled;
  digitalWrite(kRelayPin, enabled ? kRelayActiveLevel : kRelayIdleLevel);
  Serial.println(enabled ? "relay on" : "relay off");
}
}  // namespace

void setup() {
  Serial.begin(115200);
  pinMode(kRelayPin, OUTPUT);
  setRelayState(false);
  Serial.println("relay click test booted");
}

void loop() {
  const unsigned long now = millis();
  if (now - lastToggleAt >= kToggleIntervalMs) {
    lastToggleAt = now;
    setRelayState(!relayEnabled);
  }
}
