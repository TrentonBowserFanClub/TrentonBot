#include "dynamixel_motor.h"

DynamixelMotor::DynamixelMotor(int id, Eigen::Vector2d position, bool inverted)
    : id_(id), position_(position), inverted_(inverted) {
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
