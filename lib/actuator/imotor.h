#pragma once

#include "lib/math/pid.h"
#include "lib/math/vector_math.h"
#include <Eigen/Eigen>

enum MotorStatus { UNINITIALIZED = 0, INITIALIZED, FAILED };

class IMotor {
protected:
  int id_;
  Pose2D location_;
  bool inverted_;
  bool initialized_;

  const float MAX_SPEED = 100.;

public:
  // TODO treat motors as singletons once instantiated
  // Have a single method for retrieving instances
  IMotor(int id, Pose2D location, bool inverted)
      : id_(id), location_(location), inverted_(inverted){};
  virtual bool Initialize() = 0;
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
  virtual float GetMaxSpeed() = 0;
  virtual Pose2D GetMotorLocation() = 0;
  virtual MotorStatus GetStatus() = 0;
};