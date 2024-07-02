#include "motor.h"
#include <Eigen/Eigen>
#include <string>

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
  int id_;
  Eigen::Vector2d position_;
  bool inverted_;

  DynamixelConfig config_;
  bool initialized_;

  bool ValidateCommand_();

  bool WriteBytes_(int offset, int size, int value);

  int ReadBytes_(int offset, int size);

  int GetMemoryProperty_(std::string property);

  void SetMemoryProperty_(std::string property, int value);

public:
  DynamixelMotor(int id, Eigen::Vector2d position, bool inverted = false);

  bool Initialize();

  bool Configure();

  int GetSpeed();

  void SetSpeed(int speed);

  int GetPosition();

  void SetPosition(int position);

  int GetAcceleration();

  void SetAcceleration(int acceleration);

  PIDGain GetPIDGain();

  void SetPIDGain(PIDGain pid_gain);

  bool GetLED();

  void SetLED(bool enabled);

  bool GetEnabled();

  void SetEnabled(bool enabled);
};