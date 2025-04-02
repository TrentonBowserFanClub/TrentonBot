#pragma once

#include <string>

class Subsystem {
private:
public:
  // Subsystem configuration
  virtual bool periodic() { return true; };
  virtual bool simulationPeriodic() { return true; };
  std::string getName() { return ""; };

  // Command configuration
  virtual bool setDefaultCommand();
};