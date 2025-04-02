#pragma once

#include "lib/actuator/dynamixel_motor.h"
#include "lib/kinematics/drivetrain/ikinematics.h"
#include <Eigen/Eigen>
#include <time.h>

class Robot {
private:
  std::unique_ptr<DynamixelMotor> front_left;
  std::unique_ptr<DynamixelMotor> front_right;
  std::unique_ptr<DynamixelMotor> rear_left;
  std::unique_ptr<DynamixelMotor> rear_right;

public:
  Robot();

  bool Initialize();

  bool SetVelocity(Pose2Ddesired_movement);
};