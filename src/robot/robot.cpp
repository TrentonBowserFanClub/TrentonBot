#include "robot.h"

Robot::Robot(IDrivetrainKinematics *kinematics) : kinematics_(kinematics) {}

bool Robot::Initialize() {
  dynamixel::PortHandler *port_handler =
      dynamixel::PortHandler::getPortHandler("/dev/ttyUSB0");

  // Open port
  port_handler->openPort();

  // Override default baud rate
  port_handler->setBaudRate(1000000);

  std::vector<IMotor *> motor_list;
  if (!kinematics_->GetMotorList(&motor_list)) {
    return false;
  }

  // Init each motor
  // TODO this should move to whatever is controlling the motors
  for (auto motor : motor_list) {
    MotorStatus status;
    if (!motor->GetStatus(&status)) {
      return false;
    }

    // Fail out if any of the motors aren't successfully initialized
    if (status != MotorStatus::INITIALIZED) {
      return false;
    }
  }
  return true;
}

bool Robot::SetVelocity(Movement desired_movement) {
  // Get the current time in nanoseconds for kinematics to use
  timespec current_time;
  clock_gettime(CLOCK_MONOTONIC, &current_time);
  unsigned int current_time_ns =
      (current_time.tv_sec * 10 ^ 9) + current_time.tv_nsec;

  kinematics_->InverseKinematics(current_time_ns, desired_movement);

  return true;
}