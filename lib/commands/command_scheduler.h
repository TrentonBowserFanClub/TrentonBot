#pragma once

#include "command.h"
#include "lib/subsystems/subsystem.h"
#include <memory>
#include <unordered_map>

class CommandScheduler {
private:
  static std::shared_ptr<CommandScheduler> instance_;

  std::unordered_map<Subsystem, Command> active_requirements_;
  std::unordered_map<Subsystem, Command> registered_subsystems_;

  std::unordered_set<Command> scheduled_commands_;

  void schedule(Command command) {}

public:
  static std::shared_ptr<CommandScheduler> getInstance() {
    if (instance_ == nullptr) {
      instance_ = std::make_shared<CommandScheduler>();
    }
    return instance_;
  }

  void schedule(std::unordered_set<Command> commands) {
    for (Command c : commands) {
      schedule(c);
    }
  }

  void run() {
    for (std::pair<Subsystem, Command> s : registered_subsystems_) {
      s.first.periodic();

      // If simulation, run simulation periodic

      // Pet watchdog
    }
  }
};