#include "x_drive_kinematics.h"

bool XDriveKinematics::ForwardKinematics(uint64_t current_time_ns,
                                         Pose2D *out_location) {
  return true;
}

bool XDriveKinematics::InverseKinematics(uint64_t current_time_ns,
                                         Pose2D desired_movement) {
  return true;
}