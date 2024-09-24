#pragma once

#include "imotor.h"
#include "src/drivers//dynamixel/dynamixel_sdk.h"
#include "src/drivers/dynamixel/protocol1_packet_handler.h"
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
  const size_t MAX_INIT_ATTEMPTS = 3;

  DynamixelMemoryConfig config_;
  MotorStatus status_ = MotorStatus::UNINITIALIZED;
  dynamixel::PortHandler *port_handler_;
  dynamixel::PacketHandler *packet_handler_;

  bool Initialize_();

  bool ReadBytes_(MemoryLayout memory, int *out_bytes);

  bool WriteBytes_(MemoryLayout memory, int value);

  bool GetTorqueLimit_(int *out_torque_limit);

  bool SetTorqueLimit_(int torque_limit);

public:
  DynamixelMotor(int id, Location location,
                 dynamixel::PortHandler *port_handler,
                 dynamixel::PacketHandler *packet_handler,
                 bool inverted = false);

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

  virtual bool GetMaxSpeed(float *out_speed);

  virtual bool GetMotorLocation(Location *out_location);

  virtual bool GetStatus(MotorStatus *out_status);

  bool RawSpeedToNormalizedSpeed(float speed, float *out_speed);

  bool NormalizedSpeedToRawSpeed(float speed, float *out_speed);
};