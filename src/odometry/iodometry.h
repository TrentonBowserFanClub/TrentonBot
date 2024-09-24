#pragma once

#include "src/math/vector_math.h"
#include "src/robot/actuator/imotor.h"
#include <Eigen/Eigen>

class IKinematics {
public:
  /**
   * @brief Forward kinematics uses the position of actuators and/or
   * sensors on the robot, to determine the overall position of the
   * robot.
   *
   * @return true
   * @return false
   */
  virtual bool CalculateForwardKinematics(int dT, Location *out_location,
                                          Eigen::Rotation2Df *out_rotation) = 0;

  virtual bool
  CalculateInverseKinematics(int dT, Movement desired_movement,
                             std::vector<IMotor *> motor_list, ) = 0;
};
