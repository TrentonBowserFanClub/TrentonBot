#include "dynamixel_motor.h"
#include "gtest/gtest.h"
#include <Eigen/Eigen>

namespace {

const float FLOAT_EQUALITY_TOLERANCE = 1e-4;

bool GetZeroTestPairs(std::vector<std::pair<float, float>> *out_test_pairs) {
  *out_test_pairs = {
      // Confirm that 0 and 1024 raw speeds map to 0 normalized speed
      std::make_pair(0, 0),
      std::make_pair(1024, 0),
  };

  return true;
};

bool GetNonZeroTestPairs(float max_speed, bool inverted,
                         std::vector<std::pair<float, float>> *out_test_pairs) {
  int inverted_multiplier = inverted ? -1 : 1;

  *out_test_pairs = {
      // Confirm that speeds between 0 and 1024 exclusive map to
      // appropriate normalized speeds. Non-inverted motors expect
      // raw speeds less than 1024 to result in negative normalized
      // speeds, and inverted motors expect positive speeds.
      std::make_pair(100, inverted_multiplier * -(100. / 1023) * max_speed),
      std::make_pair(256, inverted_multiplier * -(256. / 1023) * max_speed),
      std::make_pair(350, inverted_multiplier * -(350. / 1023) * max_speed),
      std::make_pair(512, inverted_multiplier * -(512. / 1023) * max_speed),
      std::make_pair(600, inverted_multiplier * -(600. / 1023) * max_speed),
      std::make_pair(768, inverted_multiplier * -(768. / 1023) * max_speed),
      std::make_pair(1023, inverted_multiplier * -max_speed),
      // Also confirm that speeds between 1025 and 2047 inclusive
      // map to appropriate normalized speeds. Non-inverted motors
      // expect raw speeds greater than 1024 to result in positive
      // normalized speeds, and inverted motors expect negative speeds.
      std::make_pair(1024 + 100,
                     inverted_multiplier * (100. / 1023) * max_speed),
      std::make_pair(1024 + 256,
                     inverted_multiplier * (256. / 1023) * max_speed),
      std::make_pair(1024 + 350,
                     inverted_multiplier * (350. / 1023) * max_speed),
      std::make_pair(1024 + 512,
                     inverted_multiplier * (512. / 1023) * max_speed),
      std::make_pair(1024 + 600,
                     inverted_multiplier * (600. / 1023) * max_speed),
      std::make_pair(1024 + 768,
                     inverted_multiplier * (768. / 1023) * max_speed),
      std::make_pair(1024 + 1023, inverted_multiplier * max_speed)};

  return true;
};

TEST(DynamixelMotorTest, TestZeroRawSpeedToNormalizedSpeed) {
  DynamixelMotor motor(0, {Eigen::Vector2d(0, 0), Eigen::Rotation2Df(0)});

  {
    // Confirm that 0 and 1024 raw speeds map to 0 normalized speed.
    std::vector<std::pair<float, float>> test_pairs;
    ASSERT_TRUE(GetZeroTestPairs(&test_pairs));

    for (std::pair<float, float> test_pair : test_pairs) {
      float raw_speed = test_pair.first;
      float expected_normalized_speed = test_pair.second;
      float actual_normalized_speed;

      ASSERT_TRUE(
          motor.RawSpeedToNormalizedSpeed(raw_speed, &actual_normalized_speed));
      ASSERT_EQ(expected_normalized_speed, actual_normalized_speed);
    }
  }
}

TEST(DynamixelMotorTest, TestNonZeroRawSpeedToNormalizedSpeed) {
  DynamixelMotor motor(0, {Eigen::Vector2d(0, 0), Eigen::Rotation2Df(0)});

  float motor_max_speed;
  ASSERT_TRUE(motor.GetMaxSpeed(&motor_max_speed));

  {
    // Confirm that various raw speeds correctly map to their
    // corresponding normalized speed
    std::vector<std::pair<float, float>> test_pairs;
    ASSERT_TRUE(GetNonZeroTestPairs(motor_max_speed, false, &test_pairs));

    for (std::pair<float, float> test_pair : test_pairs) {
      float raw_speed = test_pair.first;
      float expected_normalized_speed = test_pair.second;
      float actual_normalized_speed;

      ASSERT_TRUE(
          motor.RawSpeedToNormalizedSpeed(raw_speed, &actual_normalized_speed));
      ASSERT_NEAR(expected_normalized_speed, actual_normalized_speed,
                  FLOAT_EQUALITY_TOLERANCE);
    }
  }
}

TEST(DynamixelMotorTest, TestZeroRawSpeedToNormalizedSpeedWithInvertedMotor) {
  DynamixelMotor motor(0, {Eigen::Vector2d(0, 0), Eigen::Rotation2Df(0)}, true);

  {
    // Confirm that 0 and 1024 raw speeds map to 0 normalized speed.
    std::vector<std::pair<float, float>> test_pairs;
    ASSERT_TRUE(GetZeroTestPairs(&test_pairs));

    for (std::pair<float, float> test_pair : test_pairs) {
      float raw_speed = test_pair.first;
      float expected_normalized_speed = test_pair.second;
      float actual_normalized_speed;

      ASSERT_TRUE(
          motor.RawSpeedToNormalizedSpeed(raw_speed, &actual_normalized_speed));
      ASSERT_EQ(expected_normalized_speed, actual_normalized_speed);
    }
  }
}

TEST(DynamixelMotorTest,
     TestNonZeroRawSpeedToNormalizedSpeedWithInvertedMotor) {
  DynamixelMotor motor(0, {Eigen::Vector2d(0, 0), Eigen::Rotation2Df(0)}, true);

  float motor_max_speed;
  ASSERT_TRUE(motor.GetMaxSpeed(&motor_max_speed));

  {
    // Confirm that various raw speeds correctly map to their
    // corresponding normalized speed
    std::vector<std::pair<float, float>> test_pairs;
    ASSERT_TRUE(GetNonZeroTestPairs(motor_max_speed, true, &test_pairs));

    for (std::pair<float, float> test_pair : test_pairs) {
      float raw_speed = test_pair.first;
      float expected_normalized_speed = test_pair.second;
      float actual_normalized_speed;

      ASSERT_TRUE(
          motor.RawSpeedToNormalizedSpeed(raw_speed, &actual_normalized_speed));
      ASSERT_NEAR(expected_normalized_speed, actual_normalized_speed,
                  FLOAT_EQUALITY_TOLERANCE);
    }
  }
}

TEST(DynamixelMotorTest, TestZeroNormalizedSpeedToRawSpeed) {
  DynamixelMotor motor(0, {Eigen::Vector2d(0, 0), Eigen::Rotation2Df(0)});

  {
    // Confirm that a normalized speed of 0 maps to 0 raw speed.
    // We will never send a computed raw speed of 1024 to the
    // motor, as there is a one-to-multiple mapping for normalized
    // speeds of 0.
    std::vector<std::pair<float, float>> test_pairs = {
        std::make_pair(0, 0),
    };

    for (std::pair<float, float> test_pair : test_pairs) {
      float normalized_speed = test_pair.second;
      float expected_raw_speed = test_pair.first;
      float actual_raw_speed;

      ASSERT_TRUE(
          motor.NormalizedSpeedToRawSpeed(normalized_speed, &actual_raw_speed));
      ASSERT_EQ(expected_raw_speed, actual_raw_speed);
    }
  }
}

TEST(DynamixelMotorTest, TestNonZeroNormalizedSpeedToRawSpeed) {
  DynamixelMotor motor(0, {Eigen::Vector2d(0, 0), Eigen::Rotation2Df(0)});

  float motor_max_speed;
  ASSERT_TRUE(motor.GetMaxSpeed(&motor_max_speed));

  {
    // Confirm that various normalized speeds correctly map to their
    // corresponding raw speed
    std::vector<std::pair<float, float>> test_pairs;
    ASSERT_TRUE(GetNonZeroTestPairs(motor_max_speed, false, &test_pairs));

    for (std::pair<float, float> test_pair : test_pairs) {
      float normalized_speed = test_pair.second;
      float expected_raw_speed = test_pair.first;
      float actual_raw_speed;

      ASSERT_TRUE(
          motor.NormalizedSpeedToRawSpeed(normalized_speed, &actual_raw_speed));
      ASSERT_NEAR(expected_raw_speed, actual_raw_speed,
                  FLOAT_EQUALITY_TOLERANCE);
    }
  }
}

TEST(DynamixelMotorTest, TestZeroNormalizedSpeedToRawSpeedWithInvertedMotor) {
  DynamixelMotor motor(0, {Eigen::Vector2d(0, 0), Eigen::Rotation2Df(0)}, true);

  {
    // Confirm that a normalized speed of 0 maps to 0 raw speed.
    // We will never send a computed raw speed of 1024 to the
    // motor, as there is a one-to-multiple mapping for normalized
    // speeds of 0.
    std::vector<std::pair<float, float>> test_pairs = {
        std::make_pair(0, 0),
    };

    for (std::pair<float, float> test_pair : test_pairs) {
      float normalized_speed = test_pair.second;
      float expected_raw_speed = test_pair.first;
      float actual_raw_speed;

      ASSERT_TRUE(
          motor.NormalizedSpeedToRawSpeed(normalized_speed, &actual_raw_speed));
      ASSERT_EQ(expected_raw_speed, actual_raw_speed);
    }
  }
}

TEST(DynamixelMotorTest,
     TestNonZeroNormalizedSpeedToRawSpeedWithInvertedMotor) {
  DynamixelMotor motor(0, {Eigen::Vector2d(0, 0), Eigen::Rotation2Df(0)}, true);

  float motor_max_speed;
  ASSERT_TRUE(motor.GetMaxSpeed(&motor_max_speed));

  {
    // Confirm that various normalized speeds correctly map to their
    // corresponding raw speed
    std::vector<std::pair<float, float>> test_pairs;
    ASSERT_TRUE(GetNonZeroTestPairs(motor_max_speed, true, &test_pairs));

    for (std::pair<float, float> test_pair : test_pairs) {
      float normalized_speed = test_pair.second;
      float expected_raw_speed = test_pair.first;
      float actual_raw_speed;

      ASSERT_TRUE(
          motor.NormalizedSpeedToRawSpeed(normalized_speed, &actual_raw_speed));
      ASSERT_NEAR(expected_raw_speed, actual_raw_speed,
                  FLOAT_EQUALITY_TOLERANCE);
    }
  }
}
} // namespace
