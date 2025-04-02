#pragma once

#include "lib/subsystems/subsystem.h"
#include <string>
#include <unordered_set>

class Command {
private:
  std::string name_;
  std::unordered_set<Subsystem> requirements_;

public:
  bool initialize() { return true; }
  bool execute() { return true; }
  void end(bool interrupted) {}
  bool isFinished() { return false; }
  std::unordered_set<Subsystem> getRequirements() { return requirements_; }
  void addRequirements(std::unordered_set<Subsystem> requirements) {}
  std::string getName() { return name_; }
  void setName(std::string name) { name_ = name; }
};