#include "robot.h"
#include <memory>
#include <unordered_map>
#include <unordered_set>

Robot::Robot(IDrivetrainKinematics *kinematics) : kinematics_(kinematics) {}

bool Robot::Initialize() {
  dynamixel::PortHandler *port_handler =
      dynamixel::PortHandler::getPortHandler("/dev/ttyUSB0");

  // Open port
  port_handler->openPort();

  // Override default baud rate
  port_handler->setBaudRate(1000000);

  // Get packet handler
  dynamixel::PacketHandler *packet_handler =
      dynamixel::PacketHandler::getPacketHandler(1);

  std::unordered_map<size_t, std::unique_ptr<DynamixelMotor>> motor_map;

  // Drivetrain motors
  Pose2D location = {Eigen::Vector2d(0, 0), Eigen::Rotation2Df(0)};

  this->front_left = std::make_unique<DynamixelMotor>(
      11, location, port_handler, packet_handler);
  this->front_right = std::make_unique<DynamixelMotor>(
      12, location, port_handler, packet_handler);
  this->rear_left = std::make_unique<DynamixelMotor>(10, location, port_handler,
                                                     packet_handler, true);
  this->rear_right = std::make_unique<DynamixelMotor>(
      13, location, port_handler, packet_handler, true);

  return true;
}

bool Robot::SetVelocity(Pose2D desired_movement) {
  float clamped_x = fmin(fmax(desired_movement.position.x(), -1), 1);
  float clamped_y = fmin(fmax(desired_movement.position.y(), -1), 1);
  float smallest_theta = desired_movement.rotation.smallestAngle();

  float front_left_speed =
      fmin(fmax(clamped_y + clamped_x - smallest_theta, -1), 1);
  this->front_left->SetSpeed(front_left_speed);

  float front_right_speed =
      fmin(fmax(clamped_y - clamped_x - smallest_theta, -1), 1);
  this->front_right->SetSpeed(front_right_speed);

  float rear_left_speed =
      fmin(fmax(clamped_y - clamped_x - smallest_theta, -1), 1);
  this->rear_left->SetSpeed(rear_left_speed);

  float rear_right_speed =
      fmin(fmax(clamped_y + clamped_x - smallest_theta, -1), 1);
  this->rear_right->SetSpeed(rear_right_speed);

  return true;
}