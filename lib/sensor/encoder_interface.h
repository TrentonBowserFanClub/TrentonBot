#pragma once

#include "lib/math/vector_math.h"

class IEncoder {
protected:
  int id_;
  Pose2D location_;
  bool inverted_;
  float ticks_per_inch_;

public:
  IEncoder(int id, Pose2D location, bool inverted, float ticks_per_inch)
      : id_(id), location_(location), inverted_(inverted),
        ticks_per_inch_(ticks_per_inch){};
  virtual bool Initialize() = 0;
  virtual bool Reset() = 0;
  virtual bool GetTicks(int *out_ticks) = 0;
  virtual bool GetInches(float *out_inches) = 0;
  virtual bool GetLocation(Pose2D *out_location) = 0;
};