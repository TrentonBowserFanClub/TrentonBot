#include "mock_motor.h"
#include "gtest/gtest.h"
#include <Eigen/Eigen>

namespace {
TEST(MockMotorTest, TestPositionConfiguration) {
  /// Confirm we can write and read position values.
  MockMotor motor(0, {Eigen::Vector2d(0, 0), Eigen::Rotation2Df(0)});

  // Set and read back position values
  EXPECT_TRUE(motor.SetPosition(10));

  int out;
  EXPECT_TRUE(motor.GetPosition(&out));

  EXPECT_EQ(out, 10);
}

TEST(MockMotorTest, TestSpeedConfiguration) {
  /// Confirm we can write and read speed values.
  MockMotor motor(0, {Eigen::Vector2d(0, 0), Eigen::Rotation2Df(0)});

  // Set and read back speed values
  EXPECT_TRUE(motor.SetSpeed(20));

  float out;
  EXPECT_TRUE(motor.GetSpeed(&out));

  EXPECT_EQ(out, 20);
}

TEST(MockMotorTest, TestAccelerationConfiguration) {
  /// Confirm we can write and read acceleration values.
  MockMotor motor(0, {Eigen::Vector2d(0, 0), Eigen::Rotation2Df(0)});

  // Set and read back acceleration values
  EXPECT_TRUE(motor.SetAcceleration(30));

  float out;
  EXPECT_TRUE(motor.GetAcceleration(&out));

  EXPECT_EQ(out, 30);
}

TEST(MockMotorTest, TestPIDGainConfiguration) {
  /// Confirm we can write and read PID gain values.
  MockMotor motor(0, {Eigen::Vector2d(0, 0), Eigen::Rotation2Df(0)});

  // Set and read back PID gain values
  EXPECT_TRUE(motor.SetPIDGain({1, 2, 3}));

  PIDGain out;
  EXPECT_TRUE(motor.GetPIDGain(&out));

  EXPECT_EQ(out.kP, 1);
  EXPECT_EQ(out.kI, 2);
  EXPECT_EQ(out.kD, 3);
}

TEST(MockMotorTest, TestLEDConfiguration) {
  /// Confirm we can write and read LED configuration values.
  MockMotor motor(0, {Eigen::Vector2d(0, 0), Eigen::Rotation2Df(0)});

  {
    bool out;
    EXPECT_TRUE(motor.GetLED(&out));

    EXPECT_FALSE(out);
  }

  // Set and read back LED configuration values
  EXPECT_TRUE(motor.SetLED(true));

  {
    bool out;
    EXPECT_TRUE(motor.GetLED(&out));

    EXPECT_TRUE(out);
  }
}

TEST(MockMotorTest, TestMotorConfiguration) {
  /// Confirm we can write and read motor configuration values.
  MockMotor motor(0, {Eigen::Vector2d(0, 0), Eigen::Rotation2Df(0)});

  {
    bool out;
    EXPECT_TRUE(motor.GetEnabled(&out));

    EXPECT_FALSE(out);
  }

  // Set and read back motor configuration values
  EXPECT_TRUE(motor.SetEnabled(true));

  {
    bool out;
    EXPECT_TRUE(motor.GetEnabled(&out));

    EXPECT_TRUE(out);
  }
}

TEST(MockMotorTest, TestLocationConfiguration) {
  /// Confirm we can write and read motor configuration values.
  MockMotor motor(0, {Eigen::Vector2d(1, 2), Eigen::Rotation2Df(M_PI)});

  Location out;
  EXPECT_TRUE(motor.GetMotorLocation(&out));

  EXPECT_EQ(out.position.x(), 1);
  EXPECT_EQ(out.position.y(), 2);
  EXPECT_NEAR(out.rotation.angle(), M_PI, 1e-5);
}
} // namespace
