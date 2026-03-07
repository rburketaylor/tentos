#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

#include <string>

#include "controller_config.h"
#include "grow_protocol.h"
#include "light_control.h"

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

unsigned long lastTelemetryAt = 0;
unsigned long lastWifiConnectAttemptAt = 0;
unsigned long lastMqttConnectAttemptAt = 0;
unsigned long lastSensorSampleAt = 0;
unsigned long relayPulseStartedAt = 0;
unsigned long relayPulseDurationMs = grow::config::kRelayPulseMs;
bool relayPulseActive = false;

namespace {

using grow::light::CommandStatus;
using grow::light::ObservedLightState;
using grow::light::RequestedLightState;
using grow::light::SensorThresholds;
using grow::light::SensorWindow;

struct SensorAccumulator {
  int minimum = 4095;
  int maximum = 0;
  unsigned long total = 0;
  std::size_t count = 0;

  void reset() {
    minimum = 4095;
    maximum = 0;
    total = 0;
    count = 0;
  }

  void add(int value) {
    minimum = min(minimum, value);
    maximum = max(maximum, value);
    total += static_cast<unsigned long>(value);
    count += 1;
  }

  bool ready() const {
    return count >= grow::config::kSensorSamplesPerDecision;
  }

  SensorWindow window() const {
    return SensorWindow{
        count == 0 ? 0 : static_cast<int>(total / count),
        minimum,
        maximum,
        count};
  }
};

struct PendingCommand {
  bool active = false;
  bool waiting_for_settle = false;
  RequestedLightState desired_state = RequestedLightState::kNone;
  unsigned long pulse_ms = grow::config::kRelayPulseMs;
  uint8_t attempts = 0;
  unsigned long settle_started_at = 0;
};

SensorAccumulator sensorAccumulator;
PendingCommand pendingCommand;

const SensorThresholds kSensorThresholds{
    grow::config::kSensorOnThreshold,
    grow::config::kSensorOffThreshold,
};

ObservedLightState observedLightState = ObservedLightState::kUnknown;
RequestedLightState desiredLightState = RequestedLightState::kNone;
CommandStatus commandStatus = CommandStatus::kIdle;
bool sensorStateKnownOnce = false;
int lastSensorAverage = 0;
uint8_t lastAttemptCount = 0;
String lastCommand = "boot";

int relayActiveLevel() {
  return grow::config::kRelayActiveLow ? LOW : HIGH;
}

int relayIdleLevel() {
  return grow::config::kRelayActiveLow ? HIGH : LOW;
}

bool isBlank(const char* value) {
  return value == nullptr || value[0] == '\0';
}

void setRelayOutput(bool active) {
  digitalWrite(grow::config::kRelayPin, active ? relayActiveLevel() : relayIdleLevel());
}

String escapeJson(const String& input) {
  String escaped;
  escaped.reserve(input.length());

  for (unsigned int i = 0; i < input.length(); i++) {
    const char character = input[i];
    if (character == '\\' || character == '"') {
      escaped += '\\';
    }
    escaped += character;
  }

  return escaped;
}

void publishState() {
  if (!mqttClient.connected()) {
    return;
  }

  const char* relayState = relayPulseActive ? "pulsing" : "idle";
  String payload = "{\"relay\":\"";
  payload += relayState;
  payload += "\",\"observed_light\":\"";
  payload += grow::light::observedLightStateName(observedLightState);
  payload += "\",\"desired_light\":\"";
  payload += grow::light::requestedLightStateName(desiredLightState);
  payload += "\",\"command_status\":\"";
  payload += grow::light::commandStatusName(commandStatus);
  payload += "\",\"last_command\":\"";
  payload += escapeJson(lastCommand);
  payload += "\",\"attempts\":";
  payload += lastAttemptCount;
  payload += ",\"pulse_ms\":";
  payload += relayPulseDurationMs;
  payload += ",\"sensor_avg\":";
  payload += lastSensorAverage;
  payload += "}";
  mqttClient.publish(grow::kStateTopic, payload.c_str(), true);
}

void publishAvailability(const char* status) {
  if (!mqttClient.connected()) {
    return;
  }
  mqttClient.publish(grow::kAvailabilityTopic, status, true);
}

void startRelayPulse() {
  relayPulseStartedAt = millis();
  relayPulseDurationMs = pendingCommand.pulse_ms;
  relayPulseActive = true;
  setRelayOutput(true);
  Serial.print("relay pulse start: ");
  Serial.print(grow::light::requestedLightStateName(pendingCommand.desired_state));
  Serial.print(" attempt ");
  Serial.print(pendingCommand.attempts);
  Serial.print(" for ");
  Serial.print(relayPulseDurationMs);
  Serial.println("ms");
  publishState();
}

void startNextAttempt() {
  pendingCommand.attempts += 1;
  pendingCommand.waiting_for_settle = false;
  lastAttemptCount = pendingCommand.attempts;
  commandStatus = CommandStatus::kInProgress;
  startRelayPulse();
}

void finishPendingCommand(CommandStatus status) {
  pendingCommand.active = false;
  pendingCommand.waiting_for_settle = false;
  commandStatus = status;
  publishState();
}

void beginSettleWindow() {
  pendingCommand.waiting_for_settle = true;
  pendingCommand.settle_started_at = millis();
  sensorAccumulator.reset();
  Serial.println("relay pulse end");
  publishState();
}

void processIdleSensorWindow(const SensorWindow& window) {
  lastSensorAverage = window.average;
  const ObservedLightState classified =
      grow::light::classifySensorWindow(window, kSensorThresholds);
  const bool changed =
      !sensorStateKnownOnce || classified != observedLightState;

  observedLightState = classified;
  sensorStateKnownOnce = true;
  if (changed) {
    publishState();
  }
}

void evaluateSettledSensorWindow(const SensorWindow& window) {
  lastSensorAverage = window.average;
  observedLightState = grow::light::classifySensorWindow(window, kSensorThresholds);
  publishState();

  const auto decision = grow::light::verificationDecision(
      observedLightState,
      pendingCommand.desired_state,
      pendingCommand.attempts,
      grow::config::kMaxCommandAttempts,
      millis() - pendingCommand.settle_started_at >= grow::config::kVerificationTimeoutMs);

  if (decision == grow::light::VerificationDecision::kSatisfied) {
    finishPendingCommand(CommandStatus::kCompleted);
    return;
  }

  if (decision == grow::light::VerificationDecision::kWait) {
    sensorAccumulator.reset();
    Serial.println("verification still settling, waiting before retry");
    return;
  }

  if (decision == grow::light::VerificationDecision::kRetry) {
    Serial.println("verification timed out without the requested state, retrying once");
    sensorAccumulator.reset();
    startNextAttempt();
    return;
  }

  Serial.println("verification failed after max attempts or unknown timeout");
  finishPendingCommand(CommandStatus::kFailed);
}

void sampleSensor() {
  const unsigned long now = millis();
  if (now - lastSensorSampleAt < grow::config::kSensorSampleIntervalMs) {
    return;
  }

  lastSensorSampleAt = now;
  if (relayPulseActive) {
    return;
  }

  const int value = analogRead(grow::config::kSensorPin);
  sensorAccumulator.add(value);

  if (!sensorAccumulator.ready()) {
    return;
  }

  if (pendingCommand.waiting_for_settle &&
      now - pendingCommand.settle_started_at < grow::config::kSensorSettleMs) {
    return;
  }

  const SensorWindow window = sensorAccumulator.window();
  sensorAccumulator.reset();

  if (pendingCommand.waiting_for_settle) {
    evaluateSettledSensorWindow(window);
    return;
  }

  processIdleSensorWindow(window);
}

void handleCommand(char* topic, byte* payload, unsigned int length) {
  if (String(topic) != grow::kCommandTopic) {
    return;
  }

  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += static_cast<char>(payload[i]);
  }
  message.trim();

  const auto parsed = grow::light::parseCommand(
      std::string(message.c_str()),
      grow::config::kRelayPulseMs);

  if (!parsed.valid) {
    commandStatus = CommandStatus::kIgnored;
    lastCommand = message;
    publishState();
    Serial.print("ignored command: ");
    Serial.println(message);
    return;
  }

  if (pendingCommand.active || relayPulseActive) {
    commandStatus = CommandStatus::kIgnored;
    lastCommand = message;
    publishState();
    Serial.print("ignored command while busy: ");
    Serial.println(message);
    return;
  }

  lastCommand = parsed.normalized.c_str();
  desiredLightState = parsed.desired_state;
  relayPulseDurationMs = parsed.pulse_ms;
  lastAttemptCount = 0;

  if (grow::light::matches(observedLightState, desiredLightState)) {
    commandStatus = CommandStatus::kCompleted;
    publishState();
    Serial.print("already at requested state: ");
    Serial.println(grow::light::requestedLightStateName(desiredLightState));
    return;
  }

  pendingCommand.active = true;
  pendingCommand.waiting_for_settle = false;
  pendingCommand.desired_state = parsed.desired_state;
  pendingCommand.pulse_ms = parsed.pulse_ms;
  pendingCommand.attempts = 0;
  pendingCommand.settle_started_at = 0;
  commandStatus = CommandStatus::kInProgress;
  startNextAttempt();
}

void ensureWifiConnected() {
  if (WiFi.status() == WL_CONNECTED) {
    return;
  }

  const unsigned long now = millis();
  if (now - lastWifiConnectAttemptAt < grow::config::kWifiReconnectIntervalMs) {
    return;
  }

  lastWifiConnectAttemptAt = now;
  Serial.print("connecting wifi to ");
  Serial.println(grow::config::kWifiSsid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(grow::config::kWifiSsid, grow::config::kWifiPassword);
}

bool connectMqttClient() {
  if (isBlank(grow::config::kMqttUsername)) {
    return mqttClient.connect(
        grow::kNodeId,
        grow::kAvailabilityTopic,
        0,
        true,
        "offline");
  }

  return mqttClient.connect(
      grow::kNodeId,
      grow::config::kMqttUsername,
      grow::config::kMqttPassword,
      grow::kAvailabilityTopic,
      0,
      true,
      "offline");
}

void ensureMqttConnected() {
  if (WiFi.status() != WL_CONNECTED || mqttClient.connected()) {
    return;
  }

  const unsigned long now = millis();
  if (now - lastMqttConnectAttemptAt < grow::config::kMqttReconnectIntervalMs) {
    return;
  }

  lastMqttConnectAttemptAt = now;
  Serial.print("connecting mqtt to ");
  Serial.print(grow::config::kMqttHost);
  Serial.print(":");
  Serial.println(grow::config::kMqttPort);

  if (!connectMqttClient()) {
    Serial.print("mqtt connect failed rc=");
    Serial.println(mqttClient.state());
    return;
  }

  mqttClient.subscribe(grow::kCommandTopic);
  publishAvailability("online");
  publishState();
  Serial.println("mqtt connected");
}

void updateRelayPulse() {
  if (!relayPulseActive) {
    return;
  }

  const unsigned long now = millis();
  if (now - relayPulseStartedAt < relayPulseDurationMs) {
    return;
  }

  relayPulseActive = false;
  setRelayOutput(false);
  beginSettleWindow();
}

void publishTelemetryTick() {
  if (!mqttClient.connected()) {
    return;
  }

  String payload = "{\"event\":\"telemetry_tick\",\"observed_light\":\"";
  payload += grow::light::observedLightStateName(observedLightState);
  payload += "\",\"desired_light\":\"";
  payload += grow::light::requestedLightStateName(desiredLightState);
  payload += "\",\"command_status\":\"";
  payload += grow::light::commandStatusName(commandStatus);
  payload += "\",\"sensor_avg\":";
  payload += lastSensorAverage;
  payload += "}";
  mqttClient.publish(grow::kTelemetryTopic, payload.c_str(), false);
}

}  // namespace

void setup() {
  Serial.begin(115200);
  delay(250);
  Serial.println(grow::bootMessage().c_str());
  pinMode(
      grow::config::kRelayPin,
      relayIdleLevel() == HIGH ? INPUT_PULLUP : INPUT_PULLDOWN);
  digitalWrite(grow::config::kRelayPin, relayIdleLevel());
  pinMode(grow::config::kRelayPin, OUTPUT);
  pinMode(grow::config::kSensorPin, INPUT);
  analogReadResolution(12);
  setRelayOutput(false);
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  mqttClient.setServer(grow::config::kMqttHost, grow::config::kMqttPort);
  mqttClient.setCallback(handleCommand);
  Serial.print("relay pin: ");
  Serial.println(grow::config::kRelayPin);
  Serial.print("sensor pin: ");
  Serial.println(grow::config::kSensorPin);
}

void loop() {
  ensureWifiConnected();
  ensureMqttConnected();
  mqttClient.loop();
  updateRelayPulse();
  sampleSensor();

  const unsigned long now = millis();
  if (now - lastTelemetryAt >= grow::config::kTelemetryIntervalMs) {
    lastTelemetryAt = now;
    Serial.println("telemetry tick");
    publishTelemetryTick();
  }
}
