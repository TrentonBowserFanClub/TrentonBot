#include "mock_motor.h"

MockMotor::MockMotor(int id, Pose2D location, bool inverted)
    : IMotor(id, location, inverted){};

bool MockMotor::Initialize() {
  initialized_ = true;
  return true;
}

bool MockMotor::GetPosition(int *out_position) {
  *out_position = position_;
  return true;
}

bool MockMotor::SetPosition(int position) {
  position_ = position;
  return true;
}

bool MockMotor::GetSpeed(float *out_speed) {
  *out_speed = speed_;
  return true;
}

bool MockMotor::SetSpeed(float speed) {
  speed_ = speed;
  return true;
}

bool MockMotor::GetAcceleration(float *out_acceleration) {
  *out_acceleration = acceleration_;
  return true;
}

bool MockMotor::SetAcceleration(float acceleration) {
  acceleration_ = acceleration;
  return true;
}

bool MockMotor::GetPIDGain(PIDGain *out_pid_gain) {
  *out_pid_gain = pid_gain_;
  return true;
}

bool MockMotor::SetPIDGain(PIDGain pid_gain) {
  pid_gain_ = pid_gain;
  return true;
}

bool MockMotor::GetLED(bool *out_enabled) {
  *out_enabled = led_enable_;
  return true;
}

bool MockMotor::SetLED(bool enabled) {
  led_enable_ = enabled;
  return true;
}

bool MockMotor::GetEnabled(bool *out_enabled) {
  *out_enabled = motor_enable_;
  return true;
}

bool MockMotor::SetEnabled(bool enabled) {
  motor_enable_ = enabled;
  return true;
}

float MockMotor::GetMaxSpeed() { return MAX_SPEED; }

Pose2D MockMotor::GetMotorLocation() { return location_; }

MotorStatus MockMotor::GetStatus() { return MotorStatus::INITIALIZED; }