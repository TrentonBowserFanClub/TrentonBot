#pragma once

#include "imotor.h"
#include <Eigen/Eigen>

class MockMotor : IMotor {
private:
  /*
  Since we're mocking a motor, we don't have a hardware input
  that provides us values for location and status. We mock those
  out to reasonable values here.

  Right now, I don't simulate motion because that's hard.
  */
  int position_ = 0;
  float speed_ = 0;
  float acceleration_ = 0;
  PIDGain pid_gain_ = {0, 0, 0};
  bool led_enable_ = false;
  bool motor_enable_ = false;

public:
  MockMotor(int id, Location location, bool inverted = false);

  virtual bool GetPosition(int *out_position);

  virtual bool SetPosition(int position);

  virtual bool GetSpeed(float *out_speed);

  virtual bool SetSpeed(float speed);

  virtual bool GetAcceleration(float *out_acceleration);

  virtual bool SetAcceleration(float acceleration);

  virtual bool GetPIDGain(PIDGain *out_pid_gain);

  virtual bool SetPIDGain(PIDGain pid_gain);

  virtual bool GetLED(bool *out_enabled);

  virtual bool SetLED(bool enabled);

  virtual bool GetEnabled(bool *out_enabled);

  virtual bool SetEnabled(bool enabled);

  virtual bool GetMaxSpeed(float *out_speed);

  virtual bool GetMotorLocation(Location *out_location);
};