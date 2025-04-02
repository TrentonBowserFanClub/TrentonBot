#include "drivetrain_velocity_kinematics.h"

bool DrivetrainVelocityInverseKinematics(
    std::vector<std::unique_ptr<IMotor>> motor_list, uint64_t current_time_ns,
    Pose2D desired_movement) {
  // Each motor has a position and orientation (rotation) respective
  // to the center of rotation on the robot. For each motor, we need to
  // transform the desired movement in the robot frame to a movement
  // in the motor's frame.
  for (std::unique_ptr<IMotor> &motor : motor_list) {
    Pose2D motor_location;

    if (!motor->GetMotorLocation(&motor_location))
      return false;

    // Take a unit vector and rotate by the orientation of the current wheel.
    // This vector now points in the direction of travel for the wheel.
    Eigen::Vector2d wheel_direction =
        Eigen::Vector2d(1, 0) * motor_location.rotation;

    // Now we normalize the vector for where we want to go. This will force
    // the vector's magnitude to be 1 in the same direction.
    Eigen::Vector2d normalized_desired_movement = desired_movement.position;
    normalized_desired_movement.normalize();
  }

  return true;
}