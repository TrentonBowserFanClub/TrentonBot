#pragma once

#include "src/math/vector_math.h"
#include <Eigen/Eigen>

class IForwardKinematics {
protected:
  uint64_t prev_time_ns;

public:
  IForwardKinematics(){};

  /**
   * @brief Forward kinematics uses the position of actuators and/or
   * sensors on the robot, to determine the overall position of the
   * robot.
   *
   * @return true
   * @return false
   */
  virtual bool ForwardKinematics(uint64_t current_time_ns,
                                 Pose2D *out_location) = 0;
};

class IInverseKinematics {
protected:
  uint64_t prev_time_ns;

public:
  IInverseKinematics(){};

  /**
   * @brief Inverse kinematics takes in a desired movement of the robot,
   * and translates the general movement into constituent movements for the
   * actuators on the robot.
   *
   * @param current_time_ns Current time in nanoseconds
   * @param desired_movement Vector indicating the movement desired for the
   * robot. Movement will be scaled between bounds of -1 to 1.
   * @return true
   * @return false
   */
  virtual bool InverseKinematics(uint64_t current_time_ns,
                                 Pose2D desired_movement) = 0;
};