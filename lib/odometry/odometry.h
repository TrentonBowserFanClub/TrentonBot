#pragma once

#include "lib/sensor/encoder_interface.h"
#include "lib/sensor/gyro_interface.h"
#include <Eigen/Eigen>
#include <memory>

class Odometry {
private:
  std::unique_ptr<IEncoder> x_encoder_, y_encoder_;
  std::unique_ptr<IGyro> gyro_;

  float prev_x_encoder_inches_, prev_y_encoder_inches_;
  Eigen::Rotation2Df prev_gyro_yaw_radians_;

  // Store state (provided and calculated) upon initialization
  Pose2D x_encoder_location_, y_encoder_location_;
  float x_encoder_tangent_coef_, y_encoder_tangent_coef_;

public:
  Odometry(std::unique_ptr<IEncoder> x_encoder,
           std::unique_ptr<IEncoder> y_encoder, std::unique_ptr<IGyro> gyro);

  bool Update();

  bool Reset();
};