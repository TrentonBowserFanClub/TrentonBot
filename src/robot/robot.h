#pragma once

#include "src/robot/actuator/dynamixel_motor.h"
#include <Eigen/Eigen>

class Robot {
private:
  // We need to use pointers to allow polymorphism to work. We
  // cannot have a vector of references to an abstract class, as
  // an abstract class itself can never be instantiated.
  std::vector<IMotor *> motor_list_;

public:
  Robot(std::vector<IMotor *> motor_list);

  bool SetSpeed(Eigen::Vector2d unit_speed, Eigen::Rotation2Df rotation);
};