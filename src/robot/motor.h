#include "pid.h"

class IMotor {
public:
  IMotor(){};
  virtual bool Initialize() = 0;
  virtual bool Configure() = 0;
  virtual int GetSpeed() = 0;
  virtual void SetSpeed(int speed) = 0;
  virtual int GetPosition() = 0;
  virtual void SetPosition(int position) = 0;
  virtual int GetAcceleration() = 0;
  virtual void SetAcceleration(int acceleration) = 0;
  virtual PIDGain GetPIDGain() = 0;
  virtual void SetPIDGain(PIDGain pid_gain) = 0;
  virtual bool GetLED() = 0;
  virtual void SetLED(bool enabled) = 0;
  virtual bool GetEnabled() = 0;
  virtual void SetEnabled(bool enabled) = 0;
};