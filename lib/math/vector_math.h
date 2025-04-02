#pragma once

#include <Eigen/Eigen>

// Struct to hold a position (as a vector from the origin) and a rotation of
// an object in 2D space.
struct Pose2D {
  Eigen::Vector2d position;
  Eigen::Rotation2Df rotation;
};
