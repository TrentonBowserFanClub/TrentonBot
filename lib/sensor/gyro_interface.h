#pragma once

#include "lib/math/vector_math.h"
#include <Eigen/Eigen>

struct GyroRPY {
  Eigen::Rotation2Df roll;
  Eigen::Rotation2Df pitch;
  Eigen::Rotation2Df yaw;
};

class IGyro {
protected:
  int id_;
  Pose2D location_;

public:
  IGyro(int id, Pose2D location) : id_(id), location_(location){};
  virtual bool Initialize() = 0;
  virtual bool Calibrate() = 0;
  virtual bool GetRPY(GyroRPY *out_rpy) = 0;
  virtual bool Reset() = 0;
};