#pragma once

#include "src/math/pid.h"
#include "src/math/vector_math.h"
#include <Eigen/Eigen>

class IMotor {
protected:
  int id_;
  Location location_;
  bool inverted_;

  const float MAX_SPEED = 100.;

public:
  IMotor(int id, Location location, bool inverted)
      : id_(id), location_(location), inverted_(inverted){};
  virtual bool GetPosition(int *out_position) = 0;
  virtual bool SetPosition(int position) = 0;
  virtual bool GetSpeed(float *out_speed) = 0;
  virtual bool SetSpeed(float speed) = 0;
  virtual bool GetAcceleration(float *out_acceleration) = 0;
  virtual bool SetAcceleration(float acceleration) = 0;
  virtual bool GetPIDGain(PIDGain *out_pid_gain) = 0;
  virtual bool SetPIDGain(PIDGain pid_gain) = 0;
  virtual bool GetLED(bool *out_enabled) = 0;
  virtual bool SetLED(bool enabled) = 0;
  virtual bool GetEnabled(bool *out_enabled) = 0;
  virtual bool SetEnabled(bool enabled) = 0;
  virtual bool GetMaxSpeed(float *out_speed) = 0;
  virtual bool GetMotorLocation(Location *out_location) = 0;
};