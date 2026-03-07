#include <Arduino.h>

namespace {
constexpr int kRelayPin = 18;
constexpr int kSensorPin = 34;
constexpr int kRelayActiveLevel = LOW;
constexpr int kRelayIdleLevel = HIGH;
constexpr unsigned long kRelayPulseMs = 500;
constexpr unsigned long kToggleIntervalMs = 8000;
constexpr unsigned long kSampleIntervalMs = 200;
constexpr size_t kSamplesPerLine = 8;

bool relayPulseActive = false;
unsigned long relayPulseStartedAt = 0;
unsigned long lastToggleAt = 0;
unsigned long lastSampleAt = 0;

int sampleMin = 4095;
int sampleMax = 0;
unsigned long sampleTotal = 0;
size_t sampleCount = 0;

void setRelay(bool active) {
  digitalWrite(kRelayPin, active ? kRelayActiveLevel : kRelayIdleLevel);
}

void startRelayPulse() {
  relayPulseStartedAt = millis();
  relayPulseActive = true;
  setRelay(true);
  Serial.println("relay pulse start");
}

void stopRelayPulse() {
  relayPulseActive = false;
  setRelay(false);
  Serial.println("relay pulse end");
}

void printSampleSummary() {
  if (sampleCount == 0) {
    return;
  }

  const unsigned long average = sampleTotal / sampleCount;
  Serial.print("sensor avg=");
  Serial.print(average);
  Serial.print(" min=");
  Serial.print(sampleMin);
  Serial.print(" max=");
  Serial.print(sampleMax);
  Serial.print(" relay=");
  Serial.println(relayPulseActive ? "pulse" : "idle");

  sampleMin = 4095;
  sampleMax = 0;
  sampleTotal = 0;
  sampleCount = 0;
}
}  // namespace

void setup() {
  Serial.begin(115200);
  delay(250);
  pinMode(kRelayPin, OUTPUT);
  pinMode(kSensorPin, INPUT);
  analogReadResolution(12);
  setRelay(false);
  lastToggleAt = millis();
  lastSampleAt = millis();
  Serial.println("sensor probe booted");
  Serial.println("GPIO34 reads the taped photoresistor divider");
  Serial.println("GPIO18 pulses the relay every 8 seconds for 500ms");
}

void loop() {
  const unsigned long now = millis();

  if (!relayPulseActive && now - lastToggleAt >= kToggleIntervalMs) {
    lastToggleAt = now;
    startRelayPulse();
  }

  if (relayPulseActive && now - relayPulseStartedAt >= kRelayPulseMs) {
    stopRelayPulse();
  }

  if (now - lastSampleAt >= kSampleIntervalMs) {
    lastSampleAt = now;
    const int value = analogRead(kSensorPin);
    sampleMin = min(sampleMin, value);
    sampleMax = max(sampleMax, value);
    sampleTotal += static_cast<unsigned long>(value);
    sampleCount += 1;

    if (sampleCount >= kSamplesPerLine) {
      printSampleSummary();
    }
  }
}
