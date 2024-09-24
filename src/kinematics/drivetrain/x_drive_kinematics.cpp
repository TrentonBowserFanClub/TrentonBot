#include "x_drive_kinematics.h"

XDriveKinematics::XDriveKinematics(std::vector<IMotor *> motor_list)
    : IDrivetrainKinematics(motor_list) {}

bool XDriveKinematics::ForwardKinematics(unsigned int current_time_ns,
                                         Location *out_location) {
  return true;
}

bool XDriveKinematics::InverseKinematics(unsigned int current_time_ns,
                                         Movement desired_movement) {
  return true;
}