#pragma once

#include "src/math/vector_math.h"
#include "src/robot/actuator/imotor.h"
#include <Eigen/Eigen>

class IDrivetrainKinematics {
protected:
  std::vector<IMotor *> motor_list_;

  unsigned int prev_time_ns;

public:
  IDrivetrainKinematics(std::vector<IMotor *> motor_list)
      : motor_list_(motor_list){};
  /**
   * @brief Forward kinematics uses the position of actuators and/or
   * sensors on the robot, to determine the overall position of the
   * robot.
   *
   * @return true
   * @return false
   */
  virtual bool ForwardKinematics(unsigned int current_time_ns,
                                 Location *out_location) = 0;

  /**
   * @brief Inverse kinematics takes in a desired movement of the robot,
   * and translates the general movement into constituent movements for the
   * actuators on the robot.
   *
   * @param dT
   * @param desired_movement
   * @param motor_list
   * @return true
   * @return false
   */
  virtual bool InverseKinematics(unsigned int current_time_ns,
                                 Movement desired_movement) = 0;

  virtual bool GetMotorList(std::vector<IMotor *> *out_motor_list) {
    *out_motor_list = motor_list_;
    return true;
  }
};
