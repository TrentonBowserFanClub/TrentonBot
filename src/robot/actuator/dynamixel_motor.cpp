#include "dynamixel_motor.h"

DynamixelMotor::DynamixelMotor(int id, Location location, bool inverted)
    : IMotor(id, location, inverted) {
  DynamixelMemoryLayout offsets = {
      24, // torque_enable
      25, // led_enable
      26, // d_gain
      27, // i_gain
      28, // p_gain
      30, // goal_position
      32, // moving_speed
      34, // torque_limit
      36, // present_position
      38, // present_speed
      40, // present_load
      42, // present_input_voltage
      43, // present_temperature
      44, // registered
      46, // moving
      47, // lock
      48, // punch
      50, // realtime_tick
      73, // goal_acceleration
  };

  DynamixelMemoryLayout sizes = {
      1, // torque_enable
      1, // led_enable
      1, // d_gain
      1, // i_gain
      1, // p_gain
      2, // goal_position
      2, // moving_speed
      2, // torque_limit
      2, // present_position
      2, // present_speed
      2, // present_load
      1, // present_input_voltage
      1, // present_temperature
      1, // registered
      1, // moving
      1, // lock
      2, // punch
      2, // realtime_tick
      1, // goal_acceleration
  };

  config_ = {offsets, sizes};
};

bool DynamixelMotor::Initialize_() { return true; }

bool DynamixelMotor::ValidateCommand_() { return true; }

bool DynamixelMotor::ReadBytes_(int offset, int size, int *out_bytes) {
  return true;
}

bool DynamixelMotor::WriteBytes_(int offset, int size, int value) {
  return true;
}

bool DynamixelMotor::GetMemoryProperty_(DynamixelPropertyType property,
                                        int *out_value) {
  return true;
}

bool DynamixelMotor::SetMemoryProperty_(DynamixelPropertyType property,
                                        int value) {
  return true;
}

bool DynamixelMotor::RawSpeedToNormalizedSpeed(float raw_speed,
                                               float *out_speed) {
  /*
  Speed is laid out in memory like this:
  0 0000000000
  | |--------|
  | |
  | -- Velocity
  -- Polarity

  Speed is returned as a value between 0-2048, where:
  Stopped: [0, 1024]
  CCW:     [1-1023]
  CW:      [1025-2047]

  We want to adjust the ranges to match the following:
  (TODO CHECK) CCW Full (-1023) <--- Stopped (0) ---> CW Full (1023)
  */
  float adjusted_speed;

  if (raw_speed < 1024) {
    // If we are less than the range midpoint, invert the range.
    adjusted_speed = -raw_speed;
  } else {
    // If we are in the second half of the motor's range, shift
    // the values back
    adjusted_speed = raw_speed - 1024;
  }

  /*
  Now, we want to scale the speed from this range:
  (TODO CHECK) CCW (-1023) <--- Stopped (0) ---> CW (1023)

  To match this range:
  (TODO CHECK) CCW (-100) <--- Stopped (0) ---> CW (100)
  */
  float scaled_speed = (adjusted_speed / 1023.) * MAX_SPEED;

  /*
  The last adjustment we make is to invert the speed, if requested.
  This allows for easily handling reversed motors:
  (TODO CHECK) CCW (-100) <--- Stopped (0) ---> CW (100)

  If we need to invert, will produce this range:
  (TODO CHECK) CW (-100) <--- Stopped (0) ---> CCW (100)
  */
  float inverted_speed = this->inverted_ ? -scaled_speed : scaled_speed;

  *out_speed = inverted_speed;

  return true;
}

bool DynamixelMotor::NormalizedSpeedToRawSpeed(float normalized_speed,
                                               float *out_speed) {
  /*
  The first adjustment we make is to invert the speed, if requested.
  This allows for easily handling reversed motors:
  CW (check) (-100) <--- Stopped (0) ---> CCW (100)

  If we need to invert, will produce this range:
  (TODO CHECK) CCW (-100) <--- Stopped (0) ---> CW (100)
  */
  float non_inverted_speed =
      this->inverted_ ? -normalized_speed : normalized_speed;

  /*
  Now, we want to scale the speed from this range:
  (TODO CHECK) CCW (-100) <--- Stopped (0) ---> CW (100)

  To match this range:
  (TODO CHECK) CCW (-1023) <--- Stopped (0) ---> CW (1023)
  */
  float scaled_speed = (non_inverted_speed / MAX_SPEED) * 1023.;

  /*
  The last adjustment is to convert speed from normalized form,
  to raw form.

  Normalized form (right now):
  (TODO CHECK) CCW (-1023) <--- Stopped (0) ---> CW (1023)

  Speed is laid out in memory like this:
  0 0000000000
  | |--------|
  | |
  | -- Velocity
  -- Polarity

  We want to adjust speed to match the ranges:
  Stopped: [0, 1024]
  CCW:     [1-1023]
  CW:      [1025-2047]
  */
  float adjusted_speed;

  if (scaled_speed <= 0) {
    // If we are less than or at the range midpoint, invert the range.
    // If we are at 0, we do not want to offset by 1024.
    adjusted_speed = -scaled_speed;
  } else {
    // If we are in the second half of the motor's range, shift
    // the values back
    adjusted_speed = scaled_speed + 1024;
  }

  *out_speed = adjusted_speed;

  return true;
}

bool DynamixelMotor::GetPosition(int *out_position) {
  return GetMemoryProperty_(DynamixelPropertyType::PRESENT_POSITION,
                            out_position);
}

bool DynamixelMotor::SetPosition(int position) {
  return SetMemoryProperty_(DynamixelPropertyType::GOAL_POSITION, position);
}

bool DynamixelMotor::GetSpeed(float *out_speed) {
  int raw_speed;

  if (!GetMemoryProperty_(DynamixelPropertyType::PRESENT_SPEED, &raw_speed)) {
    // Fail early if we can't read data from the motor's memory.
    return false;
  }

  return this->RawSpeedToNormalizedSpeed(static_cast<float>(raw_speed),
                                         out_speed);
}

bool DynamixelMotor::SetSpeed(float speed) {
  float raw_speed;

  if (!NormalizedSpeedToRawSpeed(speed, &raw_speed)) {
    // Fail early if we don't convert the speed correctly
    return false;
  }

  int casted_value = static_cast<int>(floor(raw_speed));

  return SetMemoryProperty_(DynamixelPropertyType::MOVING_SPEED, casted_value);
}

bool DynamixelMotor::GetAcceleration(float *out_acceleration) {
  int raw_acceleration;

  if (!GetMemoryProperty_(DynamixelPropertyType::GOAL_ACCELERATION,
                          &raw_acceleration)) {
    return false;
  }

  *out_acceleration = static_cast<float>(raw_acceleration);

  return true;
}

bool DynamixelMotor::SetAcceleration(float acceleration) { return true; }

bool DynamixelMotor::GetPIDGain(PIDGain *out_pid_gain) {
  int kP, kI, kD;

  if (GetMemoryProperty_(DynamixelPropertyType::P_GAIN, &kP) ||
      GetMemoryProperty_(DynamixelPropertyType::I_GAIN, &kI) ||
      GetMemoryProperty_(DynamixelPropertyType::D_GAIN, &kD)) {
    return false;
  }

  *out_pid_gain = {static_cast<float>(kP), static_cast<float>(kI),
                   static_cast<float>(kD)};
  return true;
}

bool DynamixelMotor::SetPIDGain(PIDGain pid_gain) {
  int kP = static_cast<int>(floor(pid_gain.kP));
  int kI = static_cast<int>(floor(pid_gain.kI));
  int kD = static_cast<int>(floor(pid_gain.kD));

  if (SetMemoryProperty_(DynamixelPropertyType::P_GAIN, kP) ||
      SetMemoryProperty_(DynamixelPropertyType::I_GAIN, kI) ||
      SetMemoryProperty_(DynamixelPropertyType::D_GAIN, kD)) {
    return false;
  }

  return true;
}

bool DynamixelMotor::GetLED(bool *out_enabled) {
  int led_enabled;

  if (!GetMemoryProperty_(DynamixelPropertyType::LED_ENABLE, &led_enabled)) {
    return false;
  }

  *out_enabled = led_enabled != 0;

  return true;
}

bool DynamixelMotor::SetLED(bool enabled) {
  return SetMemoryProperty_(DynamixelPropertyType::LED_ENABLE, enabled != 0);
}

bool DynamixelMotor::GetEnabled(bool *out_enabled) {
  int torque_enable;

  if (!GetMemoryProperty_(DynamixelPropertyType::TORQUE_ENABLE,
                          &torque_enable)) {
    return false;
  }

  *out_enabled = torque_enable != 0;

  return true;
}

bool DynamixelMotor::SetEnabled(bool enabled) {
  return SetMemoryProperty_(DynamixelPropertyType::TORQUE_ENABLE, enabled != 0);
  return true;
}

bool DynamixelMotor::GetMaxSpeed(float *out_speed) {
  *out_speed = this->MAX_SPEED;
  return true;
}

bool DynamixelMotor::GetMotorLocation(Location *out_location) {
  *out_location = this->location_;
  return true;
}