#include "odometry.h"

// Assumptions being made here:
// - Direction of movement for x encoder is (1, 0)
// - Direction of movement for y encoder is (0, 1)

Odometry::Odometry(std::unique_ptr<IEncoder> x_encoder,
                   std::unique_ptr<IEncoder> y_encoder,
                   std::unique_ptr<IGyro> gyro)
    : x_encoder_(std::move(x_encoder)), y_encoder_(std::move(y_encoder)),
      gyro_(std::move(gyro)) {
  x_encoder_->GetLocation(&x_encoder_location_);
  y_encoder_->GetLocation(&y_encoder_location_);

  // Calculate what proportion of the rotation experienced by each encoder will
  // translate to it's rotation. This is determined by looking at the vector
  // parallel with each's direction of movement, and determining how much
  // of it's component is in the direction of rotation.
  x_encoder_tangent_coef_ = fabs(sin(atan2(x_encoder_location_.position.y(),
                                           x_encoder_location_.position.x())));
  y_encoder_tangent_coef_ = fabs(cos(atan2(y_encoder_location_.position.y(),
                                           y_encoder_location_.position.x())));
}

bool Odometry::Update() {
  // Start by getting the current values for each encoder
  float x_encoder_inches, y_encoder_inches;
  this->x_encoder_->GetInches(&x_encoder_inches);
  this->y_encoder_->GetInches(&y_encoder_inches);

  GyroRPY gyro_rpy;
  this->gyro_->GetRPY(&gyro_rpy);

  // Calculate the change in position since the last check.
  float x_encoder_delta = x_encoder_inches - prev_x_encoder_inches_;
  float y_encoder_delta = y_encoder_inches - prev_y_encoder_inches_;
  Eigen::Rotation2Df gyro_delta =
      gyro_rpy.yaw * prev_gyro_yaw_radians_.inverse();

  // When the robot turns (in-place or while moving), it causes any encoder
  // that is not completely perpendicular to the direction of rotation to either
  // increase or decrease.
  //
  // We calculate the delta distance experienced due to turning in the last
  // timeframe as an arc length, then subtract from the delta distance to get
  // true distance moved.
  Pose2D x_encoder_location, y_encoder_location;
  this->x_encoder_->GetLocation(&x_encoder_location);
  this->y_encoder_->GetLocation(&y_encoder_location);

  float x_encoder_arc_length =
      x_encoder_location.position.norm() * gyro_delta.smallestAngle();
  float y_encoder_arc_length =
      y_encoder_location.position.norm() * gyro_delta.smallestAngle();

  // Each encoder will also have a different tangential component, calculated in
  // the constructor. We then multiply this by the arc length to determine the
  // expected induced rotation in each encoder.
  float x_encoder_turning_component =
      x_encoder_arc_length * x_encoder_tangent_coef_;
  float y_encoder_turning_component =
      y_encoder_arc_length * y_encoder_tangent_coef_;

  return true;
}

bool Odometry::Reset() {
  // TODO make this work
  return true;
}