# from dynamixel_sdk import *
import math
from typing import Optional
import dynamixel_sdk as dynamixel
from dataclasses import dataclass
import logging

logger = logging.getLogger(__file__)


@dataclass
class Vector2D:
    x: float
    y: float

    def __add__(self, other):
        assert isinstance(other, Vector2D)

        return Vector2D(self.x + other.x, self.y + other.y)

    def __iadd__(self, other):
        assert isinstance(other, Vector2D)

        self.x += other.x
        self.y += other.y

    def __sub__(self, other):
        assert isinstance(other, Vector2D)

        return Vector2D(self.x - other.x, self.y - other.y)

    def __isub__(self, other):
        assert isinstance(other, Vector2D)

        self.x -= other.x
        self.y -= other.y


@dataclass
class Rotation2D:
    theta: float


@dataclass
class Position2D:
    location: Vector2D
    rotation: Rotation2D

    def __init__(self, x, y, theta):
        self.location = Vector2D(x, y)
        self.rotation = Rotation2D(theta)


@dataclass
class MemorySegment:
    byte_offset: int
    byte_size: int


@dataclass
class MotorAddressConfig:
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


DYNAMIXEL_MX_12_ADDR_CONFIG = MotorAddressConfig(
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


class DynamixelMotor:
    def __init__(
        self,
        id: int,
        address_config: MotorAddressConfig,
        position: Position2D,
        inverted: bool = False,
    ):
        self.id = id
        self.address_config: MotorAddressConfig = address_config
        self.position = position
        self.inverted = inverted
        self.port_handler = None
        self.packet_handler = None
        self.initialized = False

    def __validate_command(self):
        if not self.initialized:
            raise Exception("Motor needs to be initialized before commanding")

    def __write_bytes(self, offset: int, size: int, value: int):
        if size == 1:
            dxl_comm_result, dxl_error = self.packet_handler.write1ByteTxRx(
                self.port_handler, self.id, offset, value
            )
        elif size == 2:
            dxl_comm_result, dxl_error = self.packet_handler.write2ByteTxRx(
                self.port_handler, self.id, offset, value
            )
        elif size == 4:
            dxl_comm_result, dxl_error = self.packet_handler.write4ByteTxRx(
                self.port_handler, self.id, offset, value
            )
        else:
            raise NotImplementedError

        if dxl_comm_result != dynamixel.COMM_SUCCESS:
            logger.error(
                f"FAILURE | DYNAMIXEL ID: '{self.id}' | WriteBytesDXLComm: '{self.packet_handler.getTxRxResult(dxl_comm_result)}'"
            )
            return False
        elif dxl_error != 0:
            logger.error(
                f"FAILURE | DYNAMIXEL ID: '{self.id}' | WriteBytesDXLError: '{self.packet_handler.getRxPacketError(dxl_error)}'"
            )
            return False
        return True

    def __read_bytes(self, offset: int, size: int) -> Optional[int]:
        if size == 1:
            dxl_value, dxl_comm_result, dxl_error = self.packet_handler.read1ByteTxRx(
                self.port_handler, self.id, offset
            )
        elif size == 2:
            dxl_value, dxl_comm_result, dxl_error = self.packet_handler.read2ByteTxRx(
                self.port_handler, self.id, offset
            )
        elif size == 4:
            dxl_value, dxl_comm_result, dxl_error = self.packet_handler.read4ByteTxRx(
                self.port_handler, self.id, offset
            )
        else:
            raise NotImplementedError

        if dxl_comm_result != dynamixel.COMM_SUCCESS:
            logger.error(
                f"FAILURE | DXID: '{self.id}' | ReadBytesDXLComm: '{self.packet_handler.getTxRxResult(dxl_comm_result)}'"
            )
            return False
        elif dxl_error != 0:
            logger.error(
                f"FAILURE | DXID: '{self.id}' | ReadBytesDXLError: '{self.packet_handler.getRxPacketError(dxl_error)}'"
            )
            return None
        return dxl_value

    def __get_memory_property(self, property: str) -> int:
        self.__validate_command()

        value = self.__read_bytes(
            getattr(self.address_config, property).byte_offset,
            getattr(self.address_config, property).byte_size,
        )

        if value is None:
            raise Exception(f"FATAL | DXID: '{self.id}' | Value could not be read!")

        return value

    def __set_memory_property(self, property: str, value: int):
        self.__validate_command()

        self.__write_bytes(
            getattr(self.address_config, property).byte_offset,
            getattr(self.address_config, property).byte_size,
            value,
        )

    def get_torque_enable(self) -> int:
        return self.__get_memory_property("torque_enable")

    def set_torque_enable(self, is_enabled: int):
        self.__set_memory_property("torque_enable", is_enabled)

    def get_led_enable(self) -> int:
        return self.__get_memory_property("led_enable")

    def set_led_enable(self, is_enabled: int):
        self.__set_memory_property("led_enable", is_enabled)

    def get_pid_gain(self) -> tuple[int, int, int]:
        p = self.__get_memory_property("p_gain")
        i = self.__get_memory_property("i_gain")
        d = self.__get_memory_property("d_gain")

        return (p, i, d)

    def set_pid_gain(self, p: int, i: int, d: int):
        self.__set_memory_property("p_gain", p)
        self.__set_memory_property("i_gain", i)
        self.__set_memory_property("d_gain", d)

    def get_goal_position(self) -> int:
        return self.__get_memory_property("goal_position")

    def set_goal_position(self, position: int):
        self.__set_memory_property("goal_position", position)

    def get_moving_speed(self) -> int:
        return self.__get_memory_property("moving_speed")

    def set_moving_speed(self, speed: int):
        if self.inverted:
            if speed >= 1024:
                speed -= 1024
            else:
                speed += 1024

        self.__set_memory_property("moving_speed", speed)

    def get_torque_limit(self) -> int:
        return self.__get_memory_property("torque_limit")

    def set_torque_limit(self, limit: int):
        self.__set_memory_property("torque_limit", limit)

    def get_present_position(self) -> int:
        return self.__get_memory_property("present_position")

    def get_present_speed(self) -> int:
        return self.__get_memory_property("present_speed")

    def get_present_load(self) -> int:
        return self.__get_memory_property("present_load")

    def get_present_input_voltage(self) -> int:
        return self.__get_memory_property("present_input_voltage")

    def get_present_temperature(self) -> int:
        return self.__get_memory_property("present_temperature")

    def get_moving(self) -> int:
        return self.__get_memory_property("moving")

    def get_realtime_tick(self) -> int:
        return self.__get_memory_property("realtime_tick")

    def get_goal_acceleration(self) -> int:
        return self.__get_memory_property("goal_acceleration")

    def set_goal_acceleration(self, acceleration: int):
        self.__set_memory_property("goal_acceleration")

    def init(self, port_handler, packet_handler):
        self.port_handler = port_handler
        self.packet_handler = packet_handler
        self.initialized = True

    def configure_default(self):
        # Enable LED
        self.set_led_enable(1)

        # Set default PID values intenral to the motor
        self.set_pid_gain(8, 0, 8)

        # Enable the motor, and set to 100% max torque
        self.set_torque_enable(1)
        self.set_torque_limit(1023)

        # If we've made it this far without throwing an exception,
        # we've configured our defaults successfully
        logger.info(
            f"SUCCESS | DYNAMIXEL ID: '{self.id}' | Default configuration applied!"
        )


class RobotControl:
    def __init__(
        self,
        device_name: str,
        protocol_version: int,
        motors: list[DynamixelMotor],
    ):
        self.port_handler = dynamixel.PortHandler(device_name)
        self.packet_handler = dynamixel.PacketHandler(protocol_version)
        self.motors = motors
        self.position: Vector2D = Vector2D(0, 0)

        self.__validate_motors()

    def __validate_motors(self):
        # Confirm each motor has a unique ID
        motor_ids = set([motor.id for motor in self.motors])

        if len(motor_ids) != len(self.motors):
            raise Exception("One or more motors have duplicate IDs defined!")

    def __open_port(self) -> bool:
        try:
            self.port_handler.openPort()
            logger.info(f"SUCCESS | OpenPort")
        except:
            logger.error(f"FAILURE | OpenPort")
            return False
        return True

    def __set_baud(self, baud) -> bool:
        """
        Set baud rate of the motor

        Args:
            port_handler: Object responsible for handling port communications with the Dynamixel motors

        Returns:
            bool: True if the command was sent successfully
        """
        try:
            self.port_handler.setBaudRate(baud)
            logger.info(f"SUCCESS | SetBaud: '{baud}'")
        except:
            logger.error(f"FAILURE | SetBaud: '{baud}'")
            return False
        return True

    def init(self, baud):
        if not self.__open_port():
            raise Exception("Unable to open port for Dynamixel!")

        if not self.__set_baud(baud):
            raise Exception("Unable to set baud rate for Dynamixel!")

        for motor in self.motors:
            motor.init(self.port_handler, self.packet_handler)
            motor.configure_default()

    # TODO fix the verbiage here
    def set_velocity(self, command: Position2D) -> bool:
        """
        Translates a command of unit vectors and rotations into commands for
        the motors on the robot.

        Args:
            command: RobotCommand - Command of a unit vector and rotation with the
                desired velocity of the robot.
        """
        clamped_x = min(max(command.location.x, -1), 1)
        clamped_y = min(max(command.location.y, -1), 1)
        # TODO figure out how to get the smallest angle correctly
        smallest_theta = ((command.rotation.theta + (math.pi / 2)) % math.pi) - (
            math.pi / 2
        )

        for motor in self.motors:
            # TODO actual kinematics here
            if clamped_y < 0:
                motor.set_moving_speed((abs(clamped_y) * 1023) + 1023)
            else:
                motor.set_moving_speed(clamped_y * 1023)

        return True


if __name__ == "__main__":
    """
    NOTE: This isn't for actual execution. This entrypoint is only for
    testing.
    """
    # TODO set proper locations of the motors, once determined in CAD.
    motors = [
        DynamixelMotor(10, DYNAMIXEL_MX_12_ADDR_CONFIG, Position2D(0, 0, 0)),
        DynamixelMotor(11, DYNAMIXEL_MX_12_ADDR_CONFIG, Position2D(0, 0, 0), True),
    ]

    # Set up the robot controller, with the correct baud rate for the MX12 motors
    ctrl = RobotControl("/dev/ttyUSB0", 1, motors)
    ctrl.init(1000000)

    from IPython import embed

    embed()