#pragma once

#include <string>

namespace grow {

static constexpr const char* kNodeId = "tent-controller-01";
static constexpr const char* kTelemetryTopic = "grow/tent-controller-01/telemetry";
static constexpr const char* kStateTopic = "grow/tent-controller-01/state";
static constexpr const char* kCommandTopic = "grow/tent-controller-01/command";
static constexpr const char* kAvailabilityTopic = "grow/tent-controller-01/availability";

inline std::string bootMessage() {
  return "Grow controller booting with local failsafe mode enabled.";
}

}  // namespace grow
