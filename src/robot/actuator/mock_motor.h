#include "motor.h"
#include <Eigen/Eigen>

class MockMotor : IMotor {
private:
  /*
  Since we're mocking a motor, we don't have a hardware input
  that provides us values for location and status. We mock those
  out to reasonable values here.

  Right now, I don't simulate motion because that's hard.
  */
  int position_ = 0;
  float speed_ = 0;
  float acceleration_ = 0;
  PIDGain pid_gain_ = {0, 0, 0};
  bool led_enable_ = false;
  bool motor_enable_ = false;

public:
  MockMotor(int id, Location location, bool inverted = false);

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
};