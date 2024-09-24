#pragma once

#include "src/kinematics/drivetrain/ikinematics.h"

class XDriveKinematics : IDrivetrainKinematics {
private:
public:
  XDriveKinematics(std::vector<IMotor *> motor_list);

  virtual bool ForwardKinematics(unsigned int current_time_ns,
                                 Location *out_location);

  virtual bool InverseKinematics(unsigned int current_time_ns,
                                 Movement desired_movement);
};
