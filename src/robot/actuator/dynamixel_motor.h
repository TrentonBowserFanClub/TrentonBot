#include "motor.h"
#include <Eigen/Eigen>

enum DynamixelPropertyType {
  TORQUE_ENABLE = 0,
  LED_ENABLE,
  P_GAIN,
  I_GAIN,
  D_GAIN,
  GOAL_POSITION,
  MOVING_SPEED,
  TORQUE_LIMIT,
  PRESENT_POSITION,
  PRESENT_SPEED,
  PRESENT_LOAD,
  PRESENT_INPUT_VOLTAGE,
  PRESENT_TEMPERATURE,
  REGISTERED,
  MOVING,
  LOCK,
  PUNCH,
  REALTIME_TICK,
  GOAL_ACCELERATION,
};

struct DynamixelMemoryLayout {
  int torque_enable;
  int led_enable;
  int d_gain;
  int i_gain;
  int p_gain;
  int goal_position;
  int moving_speed;
  int torque_limit;
  int present_position;
  int present_speed;
  int present_load;
  int present_input_voltage;
  int present_temperature;
  int registered;
  int moving;
  int lock;
  int punch;
  int realtime_tick;
  int goal_acceleration;
};

struct DynamixelConfig {
  DynamixelMemoryLayout offsets;
  DynamixelMemoryLayout sizes;
};

class DynamixelMotor : IMotor {
private:
  DynamixelConfig config_;
  bool initialized_;

  bool Initialize_();

  bool ValidateCommand_();

  bool ReadBytes_(int offset, int size, int *out_bytes);

  bool WriteBytes_(int offset, int size, int value);

  bool GetMemoryProperty_(DynamixelPropertyType property, int *out_value);

  bool SetMemoryProperty_(DynamixelPropertyType property, int value);

public:
  DynamixelMotor(int id, Location location, bool inverted = false);

  bool GetPosition(int *out_position);

  bool SetPosition(int position);

  bool GetSpeed(float *out_speed);

  bool SetSpeed(float speed);

  bool GetAcceleration(float *out_acceleration);

  bool SetAcceleration(float acceleration);

  bool GetPIDGain(PIDGain *out_pid_gain);

  bool SetPIDGain(PIDGain pid_gain);

  bool GetLED(bool *out_enabled);

  bool SetLED(bool enabled);

  bool GetEnabled(bool *out_enabled);

  bool SetEnabled(bool enabled);

  bool GetMaxSpeed(float *out_speed);

  bool GetMotorLocation(Location *out_location);

  bool RawSpeedToNormalizedSpeed(float speed, float *out_speed);

  bool NormalizedSpeedToRawSpeed(float speed, float *out_speed);
};