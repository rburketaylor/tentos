#pragma once

#include <cstddef>
#include <stdint.h>

namespace grow::config {

static constexpr const char* kWifiSsid = "YOUR_WIFI_SSID";
static constexpr const char* kWifiPassword = "YOUR_WIFI_PASSWORD";

static constexpr const char* kMqttHost = "192.168.0.21";
static constexpr uint16_t kMqttPort = 1883;
static constexpr const char* kMqttUsername = "";
static constexpr const char* kMqttPassword = "";

static constexpr int kRelayPin = 18;
static constexpr int kSensorPin = 34;
static constexpr bool kRelayActiveLow = true;
static constexpr unsigned long kRelayPulseMs = 500;
static constexpr int kSensorOnThreshold = 80;
static constexpr int kSensorOffThreshold = 40;
static constexpr unsigned long kSensorSampleIntervalMs = 100;
static constexpr unsigned long kSensorSettleMs = 750;
static constexpr unsigned long kVerificationTimeoutMs = 3000;
static constexpr size_t kSensorSamplesPerDecision = 8;
static constexpr uint8_t kMaxCommandAttempts = 2;

static constexpr unsigned long kWifiReconnectIntervalMs = 10000;
static constexpr unsigned long kMqttReconnectIntervalMs = 5000;
static constexpr unsigned long kTelemetryIntervalMs = 30000;

}  // namespace grow::config
