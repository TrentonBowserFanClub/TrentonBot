from dataclasses import dataclass


@dataclass
class MemorySegment:
    byte_offset: int
    byte_size: int


@dataclass
class DynamixelMotorAddressConfig:
    torque_enable: MemorySegment
    led_enable: MemorySegment
    d_gain: MemorySegment
    i_gain: MemorySegment
    p_gain: MemorySegment
    goal_position: MemorySegment
    moving_speed: MemorySegment
    torque_limit: MemorySegment
    present_position: MemorySegment
    present_speed: MemorySegment
    present_load: MemorySegment
    present_input_voltage: MemorySegment
    present_temperature: MemorySegment
    registered: MemorySegment
    moving: MemorySegment
    lock: MemorySegment
    punch: MemorySegment
    realtime_tick: MemorySegment
    goal_acceleration: MemorySegment
