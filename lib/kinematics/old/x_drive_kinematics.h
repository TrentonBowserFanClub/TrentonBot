#pragma once

#include "src/kinematics/kinematics_interfaces.h"
#include "src/robot/actuator/imotor.h"

class XDriveKinematics : IForwardKinematics, IInverseKinematics {
private:
  std::vector<std::unique_ptr<IMotor>> motor_list_;

public:
  XDriveKinematics(std::vector<std::unique_ptr<IMotor>> motor_list)
      : motor_list_(motor_list){};

  virtual bool ForwardKinematics(uint64_t current_time_ns,
                                 Pose2D *out_location);

  virtual bool InverseKinematics(uint64_t current_time_ns,
                                 Pose2D desired_movement);
};
