#include "robot.h"

Robot::Robot(std::vector<DynamixelMotor> motor_list)
    : motor_list_(motor_list) {}

bool Robot::SetSpeed(Eigen::Vector2d desired_speed,
                     Eigen::Rotation2Df rotation) {
  // We are provided a unit vector for the direction and speed,
  // and rotation to perform at the same time. We need to perform
  // the kinematics to determine what the motor speeds for each motor
  // need to be
  for (auto motor : motor_list_) {
    // Get locations for each motor
    Location motor_location;
    if (!motor.GetMotorLocation(&motor_location)) {
      return false;
    }

    // Take the dot product between where we want to go (desired_speed)
    // and the vector between the center of the robot and the wheel
    float dot_prod = motor_location.position.dot(desired_speed);

    // We then scale our desired speed vector, and rotate it to be
    // parallel with the wheel
    Eigen::Vector2d scaled_speed = desired_speed * dot_prod;
  }

  return true;
}