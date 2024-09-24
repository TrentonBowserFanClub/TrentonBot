#pragma once

#include "src/kinematics/drivetrain/ikinematics.h"
#include "src/robot/actuator/dynamixel_motor.h"
#include <Eigen/Eigen>
#include <time.h>

class Robot {
private:
  // We need to use pointers to allow polymorphism to work. We
  // cannot have a vector of references to an abstract class, as
  // an abstract class itself can never be instantiated.
  IDrivetrainKinematics *kinematics_;

public:
  Robot(IDrivetrainKinematics *kinematics_);

  bool Initialize();

  bool SetVelocity(Movement desired_movement);
};