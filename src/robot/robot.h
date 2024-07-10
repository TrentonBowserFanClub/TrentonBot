#include "src/robot/actuator/dynamixel_motor.h"
#include <Eigen/Eigen>

class Robot {
private:
  std::vector<DynamixelMotor> motor_list_;

public:
  Robot(std::vector<DynamixelMotor> motor_list);

  bool SetSpeed(Eigen::Vector2d unit_speed, Eigen::Rotation2Df rotation);
};