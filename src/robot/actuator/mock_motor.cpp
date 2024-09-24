#include "mock_motor.h"

MockMotor::MockMotor(int id, Location location, bool inverted)
    : IMotor(id, location, inverted){};

bool MockMotor::GetPosition(int *out_position) {
  *out_position = this->position_;
  return true;
}

bool MockMotor::SetPosition(int position) {
  this->position_ = position;
  return true;
}

bool MockMotor::GetSpeed(float *out_speed) {
  *out_speed = this->speed_;
  return true;
}

bool MockMotor::SetSpeed(float speed) {
  this->speed_ = speed;
  return true;
}

bool MockMotor::GetAcceleration(float *out_acceleration) {
  *out_acceleration = this->acceleration_;
  return true;
}

bool MockMotor::SetAcceleration(float acceleration) {
  this->acceleration_ = acceleration;
  return true;
}

bool MockMotor::GetPIDGain(PIDGain *out_pid_gain) {
  *out_pid_gain = this->pid_gain_;
  return true;
}

bool MockMotor::SetPIDGain(PIDGain pid_gain) {
  this->pid_gain_ = pid_gain;
  return true;
}

bool MockMotor::GetLED(bool *out_enabled) {
  *out_enabled = this->led_enable_;
  return true;
}

bool MockMotor::SetLED(bool enabled) {
  this->led_enable_ = enabled;
  return true;
}

bool MockMotor::GetEnabled(bool *out_enabled) {
  *out_enabled = this->motor_enable_;
  return true;
}

bool MockMotor::SetEnabled(bool enabled) {
  this->motor_enable_ = enabled;
  return true;
}

bool MockMotor::GetMaxSpeed(float *out_speed) {
  *out_speed = this->MAX_SPEED;
  return true;
}

bool MockMotor::GetMotorLocation(Location *out_location) {
  *out_location = this->location_;
  return true;
}

bool MockMotor::GetStatus(MotorStatus *out_status) {
  *out_status = MotorStatus::INITIALIZED;
  return true;
}