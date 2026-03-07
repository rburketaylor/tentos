#pragma once

#include <cctype>
#include <cstddef>
#include <cstdint>
#include <string>

namespace grow::light {

enum class ObservedLightState {
  kUnknown,
  kOff,
  kOn,
};

enum class RequestedLightState {
  kNone,
  kOff,
  kOn,
};

enum class CommandStatus {
  kIdle,
  kInProgress,
  kCompleted,
  kFailed,
  kIgnored,
};

enum class VerificationDecision {
  kSatisfied,
  kWait,
  kRetry,
  kFail,
};

struct SensorThresholds {
  int on_threshold;
  int off_threshold;
};

struct SensorWindow {
  int average;
  int minimum;
  int maximum;
  std::size_t samples;
};

struct ParsedCommand {
  bool valid;
  RequestedLightState desired_state;
  unsigned long pulse_ms;
  std::string normalized;
};

inline std::string trimCopy(const std::string& value) {
  std::size_t start = 0;
  while (start < value.size() &&
         std::isspace(static_cast<unsigned char>(value[start])) != 0) {
    start += 1;
  }

  std::size_t end = value.size();
  while (end > start &&
         std::isspace(static_cast<unsigned char>(value[end - 1])) != 0) {
    end -= 1;
  }

  return value.substr(start, end - start);
}

inline unsigned long clampPulseDuration(
    unsigned long duration_ms,
    unsigned long min_pulse_ms = 50,
    unsigned long max_pulse_ms = 5000) {
  if (duration_ms < min_pulse_ms) {
    return min_pulse_ms;
  }
  if (duration_ms > max_pulse_ms) {
    return max_pulse_ms;
  }
  return duration_ms;
}

inline ParsedCommand parseCommand(
    const std::string& raw_message,
    unsigned long default_pulse_ms,
    unsigned long min_pulse_ms = 50,
    unsigned long max_pulse_ms = 5000) {
  const std::string trimmed = trimCopy(raw_message);
  const std::size_t separator_at = trimmed.find(':');
  const std::string command_name =
      separator_at == std::string::npos ? trimmed : trimmed.substr(0, separator_at);

  RequestedLightState desired_state = RequestedLightState::kNone;
  if (command_name == "light_on") {
    desired_state = RequestedLightState::kOn;
  } else if (command_name == "light_off") {
    desired_state = RequestedLightState::kOff;
  } else {
    return ParsedCommand{false, RequestedLightState::kNone, default_pulse_ms, trimmed};
  }

  unsigned long pulse_ms = default_pulse_ms;
  if (separator_at != std::string::npos) {
    const std::string suffix = trimCopy(trimmed.substr(separator_at + 1));
    bool numeric = !suffix.empty();
    for (char character : suffix) {
      if (!std::isdigit(static_cast<unsigned char>(character))) {
        numeric = false;
        break;
      }
    }

    if (numeric) {
      pulse_ms = clampPulseDuration(
          std::stoul(suffix),
          min_pulse_ms,
          max_pulse_ms);
    }
  }

  return ParsedCommand{true, desired_state, pulse_ms, trimmed};
}

inline ObservedLightState classifySensorWindow(
    const SensorWindow& window,
    const SensorThresholds& thresholds) {
  if (window.samples == 0) {
    return ObservedLightState::kUnknown;
  }

  if (window.average <= thresholds.off_threshold &&
      window.maximum <= thresholds.off_threshold) {
    return ObservedLightState::kOff;
  }

  if (window.average >= thresholds.on_threshold &&
      window.minimum >= thresholds.off_threshold) {
    return ObservedLightState::kOn;
  }

  if (window.average <= thresholds.off_threshold) {
    return ObservedLightState::kOff;
  }

  if (window.average >= thresholds.on_threshold) {
    return ObservedLightState::kOn;
  }

  return ObservedLightState::kUnknown;
}

inline bool matches(ObservedLightState observed, RequestedLightState requested) {
  return (requested == RequestedLightState::kOn &&
          observed == ObservedLightState::kOn) ||
         (requested == RequestedLightState::kOff &&
          observed == ObservedLightState::kOff);
}

inline VerificationDecision verificationDecision(
    ObservedLightState observed,
    RequestedLightState requested,
    uint8_t attempts,
    uint8_t max_attempts,
    bool verification_timed_out) {
  if (matches(observed, requested)) {
    return VerificationDecision::kSatisfied;
  }

  if (!verification_timed_out) {
    return VerificationDecision::kWait;
  }

  if (attempts < max_attempts) {
    return VerificationDecision::kRetry;
  }

  return VerificationDecision::kFail;
}

inline const char* observedLightStateName(ObservedLightState state) {
  switch (state) {
    case ObservedLightState::kOff:
      return "off";
    case ObservedLightState::kOn:
      return "on";
    case ObservedLightState::kUnknown:
    default:
      return "unknown";
  }
}

inline const char* requestedLightStateName(RequestedLightState state) {
  switch (state) {
    case RequestedLightState::kOff:
      return "off";
    case RequestedLightState::kOn:
      return "on";
    case RequestedLightState::kNone:
    default:
      return "none";
  }
}

inline const char* commandStatusName(CommandStatus status) {
  switch (status) {
    case CommandStatus::kInProgress:
      return "in_progress";
    case CommandStatus::kCompleted:
      return "completed";
    case CommandStatus::kFailed:
      return "failed";
    case CommandStatus::kIgnored:
      return "ignored";
    case CommandStatus::kIdle:
    default:
      return "idle";
  }
}

}  // namespace grow::light
