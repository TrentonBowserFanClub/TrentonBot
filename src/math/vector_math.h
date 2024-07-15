#include <Eigen/Eigen>

// Struct to hold a position (as a vector from the origin) and a rotation of
// an object in 2D space.
struct Location {
  Eigen::Vector2d position;
  Eigen::Rotation2Df rotation;
};

// Movement and location are made of the same constituent parts, but are used
// in different contexts. For now, just create a typedef alias.
typedef Location Movement;