#pragma once

#include "src/kinematics/kinematics_interfaces.h"
#include "src/robot/actuator/imotor.h"

class DrivetrainVelocityInverseKinematics : IInverseKinematics {
protected:
  std::vector<std::unique_ptr<IMotor>> motor_list_;

public:
  DrivetrainVelocityInverseKinematics(
      std::vector<std::unique_ptr<IMotor>> motor_list)
      : motor_list_(motor_list){};

  /**
   * @brief Inverse kinematics takes in a desired movement of the robot,
   * and translates the general movement into constituent movements for the
   * actuators on the robot.
   *
   * @param current_time_ns Current time in nanoseconds
   * @param desired_movement Vector indicating the movement desired for the
   * robot. Movement will be scaled between bounds of -1 to 1.
   * @return true
   * @return false
   */
  virtual bool InverseKinematics(uint64_t current_time_ns,
                                 Pose2D desired_movement);
};
