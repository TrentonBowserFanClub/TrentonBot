#pragma once

#include "imotor.h"
#include "lib/drivers/dynamixel/dynamixel_sdk.h"
#include "lib/drivers/dynamixel/protocol1_packet_handler.h"
#include <Eigen/Eigen>
#include <iostream>

struct MemoryLayout {
  int offset;
  int size;
};

struct DynamixelMemoryConfig {
  MemoryLayout torque_enable;
  MemoryLayout led_enable;
  MemoryLayout d_gain;
  MemoryLayout i_gain;
  MemoryLayout p_gain;
  MemoryLayout goal_position;
  MemoryLayout moving_speed;
  MemoryLayout torque_limit;
  MemoryLayout present_position;
  MemoryLayout present_speed;
  MemoryLayout present_load;
  MemoryLayout present_input_voltage;
  MemoryLayout present_temperature;
  MemoryLayout registered;
  MemoryLayout moving;
  MemoryLayout lock;
  MemoryLayout punch;
  MemoryLayout realtime_tick;
  MemoryLayout goal_acceleration;
};

class DynamixelMotor : IMotor {
private:
  static constexpr DynamixelMemoryConfig config_ = {
      {24, 1}, // torque_enable
      {25, 1}, // led_enable
      {26, 1}, // d_gain
      {27, 1}, // i_gain
      {28, 1}, // p_gain
      {30, 2}, // goal_position
      {32, 2}, // moving_speed
      {34, 2}, // torque_limit
      {36, 2}, // present_position
      {38, 2}, // present_speed
      {40, 2}, // present_load
      {42, 1}, // present_input_voltage
      {43, 1}, // present_temperature
      {44, 1}, // registered
      {46, 1}, // moving
      {47, 1}, // lock
      {48, 2}, // punch
      {50, 2}, // realtime_tick
      {73, 1}, // goal_acceleration
  };

  bool is_smoketest_;
  const size_t MAX_INIT_ATTEMPTS = 3;
  MotorStatus status_ = MotorStatus::UNINITIALIZED;
  dynamixel::PortHandler *port_handler_;
  dynamixel::PacketHandler *packet_handler_;

  bool Initialize_();

  bool ReadBytes_(MemoryLayout memory, int *out_bytes);

  bool WriteBytes_(MemoryLayout memory, int value);

  bool GetTorqueLimit_(int *out_torque_limit);

  bool SetTorqueLimit_(int torque_limit);

public:
  DynamixelMotor(int id, Pose2D location, dynamixel::PortHandler *port_handler,
                 dynamixel::PacketHandler *packet_handler,
                 bool inverted = false, bool is_smoketest = false);

  bool Initialize();

  float NormalizedSpeedToRawSpeed(float speed);

  float RawSpeedToNormalizedSpeed(float speed);

  virtual bool GetPosition(int *out_position);

  virtual bool SetPosition(int position);

  virtual bool GetSpeed(float *out_speed);

  virtual bool SetSpeed(float speed);

  virtual bool GetAcceleration(float *out_acceleration);

  virtual bool SetAcceleration(float acceleration);

  virtual bool GetPIDGain(PIDGain *out_pid_gain);

  virtual bool SetPIDGain(PIDGain pid_gain);

  virtual bool GetLED(bool *out_enabled);

  virtual bool SetLED(bool enabled);

  virtual bool GetEnabled(bool *out_enabled);

  virtual bool SetEnabled(bool enabled);

  virtual float GetMaxSpeed();

  virtual Pose2D GetMotorLocation();

  virtual MotorStatus GetStatus();
};