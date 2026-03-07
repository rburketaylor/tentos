#include <unity.h>

#include "grow_protocol.h"
#include "light_control.h"

void test_topics_are_stable() {
  TEST_ASSERT_EQUAL_STRING("grow/tent-controller-01/telemetry", grow::kTelemetryTopic);
  TEST_ASSERT_EQUAL_STRING("grow/tent-controller-01/command", grow::kCommandTopic);
}

void test_boot_message_mentions_failsafe() {
  TEST_ASSERT_TRUE(grow::bootMessage().find("failsafe") != std::string::npos);
}

void test_parse_light_on_uses_default_pulse() {
  const auto parsed = grow::light::parseCommand("light_on", 500);
  TEST_ASSERT_TRUE(parsed.valid);
  TEST_ASSERT_EQUAL(static_cast<int>(grow::light::RequestedLightState::kOn),
                    static_cast<int>(parsed.desired_state));
  TEST_ASSERT_EQUAL_UINT32(500, parsed.pulse_ms);
}

void test_parse_light_off_accepts_override() {
  const auto parsed = grow::light::parseCommand("light_off:750", 500);
  TEST_ASSERT_TRUE(parsed.valid);
  TEST_ASSERT_EQUAL(static_cast<int>(grow::light::RequestedLightState::kOff),
                    static_cast<int>(parsed.desired_state));
  TEST_ASSERT_EQUAL_UINT32(750, parsed.pulse_ms);
}

void test_invalid_pulse_suffix_falls_back_to_default() {
  const auto parsed = grow::light::parseCommand("light_on:not-a-number", 500);
  TEST_ASSERT_TRUE(parsed.valid);
  TEST_ASSERT_EQUAL_UINT32(500, parsed.pulse_ms);
}

void test_unknown_command_is_invalid() {
  const auto parsed = grow::light::parseCommand("toggle", 500);
  TEST_ASSERT_FALSE(parsed.valid);
}

void test_classify_sensor_window_off() {
  const grow::light::SensorThresholds thresholds{80, 40};
  const grow::light::SensorWindow window{3, 0, 6, 8};
  TEST_ASSERT_EQUAL(
      static_cast<int>(grow::light::ObservedLightState::kOff),
      static_cast<int>(grow::light::classifySensorWindow(window, thresholds)));
}

void test_classify_sensor_window_on() {
  const grow::light::SensorThresholds thresholds{80, 40};
  const grow::light::SensorWindow window{128, 112, 144, 8};
  TEST_ASSERT_EQUAL(
      static_cast<int>(grow::light::ObservedLightState::kOn),
      static_cast<int>(grow::light::classifySensorWindow(window, thresholds)));
}

void test_classify_sensor_window_unknown_in_transition_band() {
  const grow::light::SensorThresholds thresholds{80, 40};
  const grow::light::SensorWindow window{62, 0, 112, 8};
  TEST_ASSERT_EQUAL(
      static_cast<int>(grow::light::ObservedLightState::kUnknown),
      static_cast<int>(grow::light::classifySensorWindow(window, thresholds)));
}

void test_verification_decision_satisfied_when_observed_matches() {
  TEST_ASSERT_EQUAL(
      static_cast<int>(grow::light::VerificationDecision::kSatisfied),
      static_cast<int>(grow::light::verificationDecision(
          grow::light::ObservedLightState::kOn,
          grow::light::RequestedLightState::kOn,
          1,
          2,
          false)));
}

void test_verification_decision_waits_until_timeout() {
  TEST_ASSERT_EQUAL(
      static_cast<int>(grow::light::VerificationDecision::kWait),
      static_cast<int>(grow::light::verificationDecision(
          grow::light::ObservedLightState::kUnknown,
          grow::light::RequestedLightState::kOn,
          1,
          2,
          false)));
  TEST_ASSERT_EQUAL(
      static_cast<int>(grow::light::VerificationDecision::kFail),
      static_cast<int>(grow::light::verificationDecision(
          grow::light::ObservedLightState::kUnknown,
          grow::light::RequestedLightState::kOn,
          2,
          2,
          true)));
}

void test_verification_decision_retries_once_after_timeout() {
  TEST_ASSERT_EQUAL(
      static_cast<int>(grow::light::VerificationDecision::kRetry),
      static_cast<int>(grow::light::verificationDecision(
          grow::light::ObservedLightState::kOff,
          grow::light::RequestedLightState::kOn,
          1,
          2,
          true)));
}

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;
  UNITY_BEGIN();
  RUN_TEST(test_topics_are_stable);
  RUN_TEST(test_boot_message_mentions_failsafe);
  RUN_TEST(test_parse_light_on_uses_default_pulse);
  RUN_TEST(test_parse_light_off_accepts_override);
  RUN_TEST(test_invalid_pulse_suffix_falls_back_to_default);
  RUN_TEST(test_unknown_command_is_invalid);
  RUN_TEST(test_classify_sensor_window_off);
  RUN_TEST(test_classify_sensor_window_on);
  RUN_TEST(test_classify_sensor_window_unknown_in_transition_band);
  RUN_TEST(test_verification_decision_satisfied_when_observed_matches);
  RUN_TEST(test_verification_decision_waits_until_timeout);
  RUN_TEST(test_verification_decision_retries_once_after_timeout);
  return UNITY_END();
}
