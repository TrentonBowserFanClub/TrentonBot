from structs import DynamixelMotorAddressConfig, MemorySegment

# Add all Dynamixel Motor Configs here
DYNAMIXEL_MX_12_ADDR_CONFIG = DynamixelMotorAddressConfig(
    torque_enable=MemorySegment(24, 1),
    led_enable=MemorySegment(25, 1),
    d_gain=MemorySegment(26, 1),
    i_gain=MemorySegment(27, 1),
    p_gain=MemorySegment(28, 1),
    goal_position=MemorySegment(30, 2),
    moving_speed=MemorySegment(32, 2),
    torque_limit=MemorySegment(34, 2),
    present_position=MemorySegment(36, 2),
    present_speed=MemorySegment(38, 2),
    present_load=MemorySegment(40, 2),
    present_input_voltage=MemorySegment(42, 1),
    present_temperature=MemorySegment(43, 1),
    registered=MemorySegment(44, 1),
    moving=MemorySegment(46, 1),
    lock=MemorySegment(47, 1),
    punch=MemorySegment(48, 2),
    realtime_tick=MemorySegment(50, 2),
    goal_acceleration=MemorySegment(73, 1),
)
