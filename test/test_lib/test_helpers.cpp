#include <Arduino.h>
#include "unity.h"
#include "../../lib/helpers.h"

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

void test_microSecondsToCentimeter_validDuration_returnsCalculatedDistance(void) {
  float expectedDepth = 8;
  long msPingDuration = 500;

  float result = microsecondsToCentimeters(msPingDuration);

  TEST_ASSERT_EQUAL_FLOAT(expectedDepth, result);
}


void setup() {
  // Wait ~2 seconds before the Unity test runner
  // establishes connection with a board Serial interface
  delay(2000);

  UNITY_BEGIN();
  RUN_TEST(test_microSecondsToCentimeter_validDuration_returnsCalculatedDistance);
}
void loop() {
  UNITY_END();
}
