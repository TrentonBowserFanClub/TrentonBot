#include "dynamixel_motor.h"

DynamixelMotor::DynamixelMotor(int id, Pose2D location,
                               dynamixel::PortHandler *port_handler,
                               dynamixel::PacketHandler *packet_handler,
                               bool inverted, bool is_smoketest)
    : IMotor(id, location, inverted), is_smoketest_(is_smoketest),
      port_handler_(port_handler), packet_handler_(packet_handler) {

  initialized_ = this->Initialize();
};

bool DynamixelMotor::Initialize_() {
  // Enable LED
  if (!SetLED(true)) {
    return false;
  }

  // Set default PID values internal to the motor (8, 0, 8)
  if (!SetPIDGain({8, 0, 8})) {
    return false;
  }

  // Enable the motor
  if (!SetEnabled(true)) {
    return false;
  }

  // Set motor to 100% output torque
  if (!SetTorqueLimit_(1023)) {
    return false;
  }

  // Print out success
  std::cout << "SUCCESS | ID: " << id_ << " Default configuration applied!"
            << std::endl;

  return true;
}

bool DynamixelMotor::ReadBytes_(MemoryLayout memory, int *out_bytes) {
  if (is_smoketest_) {
    // If running in smoketest mode, skip any hardware interfacing
    *out_bytes = 0;
    return true;
  }

  // Would be great if this was templated, rewrite sdk soon (tm)
  uint8_t out_error;
  int comm_result;

  switch (memory.size) {
  case 1: {
    uint8_t out_data;

    comm_result = packet_handler_->read1ByteTxRx(
        port_handler_, id_, memory.offset, &out_data, &out_error);
    *out_bytes = static_cast<int>(out_data);
  } break;

  case 2: {
    uint16_t out_data;

    comm_result = packet_handler_->read2ByteTxRx(
        port_handler_, id_, memory.offset, &out_data, &out_error);

    *out_bytes = static_cast<int>(out_data);
  } break;

  case 4: {
    uint32_t out_data;

    comm_result = packet_handler_->read4ByteTxRx(
        port_handler_, id_, memory.offset, &out_data, &out_error);

    *out_bytes = static_cast<int>(out_data);
  } break;

  default:
    return false;
  }

  if (comm_result != COMM_SUCCESS) {
    std::cout << "FAILURE | DXID: " << id_ << " | CommResult: " << comm_result
              << std::endl;
    return false;
  }

  if (out_error != 0) {
    std::cout << "FAILURE | DXID: " << id_ << " | ErrResult: " << out_error
              << std::endl;
    return false;
  }

  return true;
}

bool DynamixelMotor::WriteBytes_(MemoryLayout memory, int value) {
  if (is_smoketest_) {
    // If running in smoketest mode, skip any hardware interfacing
    return true;
  }

  // Would be great if this was templated, rewrite sdk soon (tm)
  uint8_t out_error;
  int comm_result;

  switch (memory.size) {
  case 1: {
    comm_result = packet_handler_->write1ByteTxRx(
        port_handler_, id_, memory.offset, value, &out_error);
  } break;

  case 2: {
    comm_result = packet_handler_->write2ByteTxRx(
        port_handler_, id_, memory.offset, value, &out_error);
  } break;

  case 4: {
    comm_result = packet_handler_->write4ByteTxRx(
        port_handler_, id_, memory.offset, value, &out_error);
  } break;

  default:
    return false;
  }

  if (comm_result != COMM_SUCCESS) {
    std::cout << "FAILURE | DXID: " << id_ << " | CommResult: " << comm_result
              << std::endl;
    return false;
  }

  if (out_error != 0) {
    std::cout << "FAILURE | DXID: " << id_ << " | ErrResult: " << out_error
              << std::endl;
    return false;
  }

  return true;
}

bool DynamixelMotor::GetTorqueLimit_(int *out_torque_limit) {
  return ReadBytes_(config_.torque_limit, out_torque_limit);
}

bool DynamixelMotor::SetTorqueLimit_(int torque_limit) {
  return WriteBytes_(config_.torque_limit, torque_limit);
}

/**
 * @brief Initialization needs to happen after our comms drivers
 * are configured.
 *
 * @return true
 * @return false
 */
bool DynamixelMotor::Initialize() {
  size_t attempts = 0;
  bool initialized_ = false;

  while (!initialized_ && attempts < MAX_INIT_ATTEMPTS) {
    // Try to initialize the motor multiple times before failing out
    initialized_ = Initialize_();
    attempts++;
  }

  if (initialized_) {
    status_ = MotorStatus::INITIALIZED;
  } else {
    status_ = MotorStatus::FAILED;
    std::cout << "FAILURE | ID: " << this->id_ << " failed to initialize!"
              << std::endl;
    return false;
  }
  return true;
}

float DynamixelMotor::NormalizedSpeedToRawSpeed(float normalized_speed) {
  /*
  The first adjustment we make is to invert the speed, if requested.
  This allows for easily handling reversed motors:
  CW (check) (-100) <--- Stopped (0) ---> CCW (100)

  If we need to invert, will produce this range:
  (TODO CHECK) CCW (-100) <--- Stopped (0) ---> CW (100)
  */
  float non_inverted_speed = inverted_ ? -normalized_speed : normalized_speed;

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

  return adjusted_speed;
}

/**
 * @brief Convert raw speed (format used by the motor) to a normalized
 * form (-100 to 100).
 *
 * @param raw_speed
 * @return true
 * @return false
 */
float DynamixelMotor::RawSpeedToNormalizedSpeed(float raw_speed) {
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
  float inverted_speed = inverted_ ? -scaled_speed : scaled_speed;

  return inverted_speed;
}

bool DynamixelMotor::GetPosition(int *out_position) {
  return ReadBytes_(config_.present_position, out_position);
}

bool DynamixelMotor::SetPosition(int position) {
  return WriteBytes_(config_.present_position, position);
}

bool DynamixelMotor::GetSpeed(float *out_speed) {
  int raw_speed;

  if (!ReadBytes_(config_.present_speed, &raw_speed)) {
    // Fail early if we can't read data from the motor's memory.
    return false;
  }

  *out_speed = RawSpeedToNormalizedSpeed(static_cast<float>(raw_speed));

  return true;
}

bool DynamixelMotor::SetSpeed(float speed) {
  float raw_speed = NormalizedSpeedToRawSpeed(speed);

  int casted_value = static_cast<int>(floor(raw_speed));

  return WriteBytes_(config_.moving_speed, casted_value);
}

bool DynamixelMotor::GetAcceleration(float *out_acceleration) {
  int raw_acceleration;

  if (!ReadBytes_(config_.goal_acceleration, &raw_acceleration)) {
    return false;
  }

  *out_acceleration = static_cast<float>(raw_acceleration);

  return true;
}

bool DynamixelMotor::SetAcceleration(float acceleration) {
  // Not implemented
  return false;
}

bool DynamixelMotor::GetPIDGain(PIDGain *out_pid_gain) {
  int kP, kI, kD;

  if (!ReadBytes_(config_.p_gain, &kP) || !ReadBytes_(config_.i_gain, &kI) ||
      !ReadBytes_(config_.d_gain, &kD)) {
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

  if (!WriteBytes_(config_.p_gain, kP) || !WriteBytes_(config_.i_gain, kI) ||
      !WriteBytes_(config_.d_gain, kD)) {
    return false;
  }

  return true;
}

bool DynamixelMotor::GetLED(bool *out_enabled) {
  int led_enabled;

  if (!ReadBytes_(config_.led_enable, &led_enabled)) {
    return false;
  }

  *out_enabled = led_enabled != 0;

  return true;
}

bool DynamixelMotor::SetLED(bool enabled) {
  return WriteBytes_(config_.led_enable, enabled != 0);
}

bool DynamixelMotor::GetEnabled(bool *out_enabled) {
  int torque_enable;

  if (!ReadBytes_(config_.torque_enable, &torque_enable)) {
    return false;
  }

  *out_enabled = torque_enable != 0;

  return true;
}

bool DynamixelMotor::SetEnabled(bool enabled) {
  return WriteBytes_(config_.torque_enable, enabled != 0);
}

float DynamixelMotor::GetMaxSpeed() { return MAX_SPEED; }

Pose2D DynamixelMotor::GetMotorLocation() { return location_; }

MotorStatus DynamixelMotor::GetStatus() { return status_; }