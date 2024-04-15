# import os
# import rclpy
# from rclpy.node import Node
# from dynamixel_sdk import *
# from std_msgs.msg import String


# # Pre-init configuration when running
# if os.name == 'nt':
#     import msvcrt
#     def getch():
#         return msvcrt.getch().decode()
# else:
#     import sys, tty, termios
#     fd = sys.stdin.fileno()
#     old_settings = termios.tcgetattr(fd)
#     def getch():
#         try:
#             tty.setraw(sys.stdin.fileno())
#             ch = sys.stdin.read(1)
#         finally:
#             termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
#         return ch


# class RobotSubscriber(Node):

#     def __init__(self):
#         super().__init__('robot_scriber')
#         self.subscription = self.create_subscription(
#             String,
#             'topic',
#             self.listener_callback,
#             10)
#         self.subscription  # prevent unused variable warning

#     def listener_callback(self, msg):
#         self.get_logger().info('I heard: "%s"' % msg.data)


# def main(args=None):
#     rclpy.init(args=args)

#     minimal_subscriber = MinimalSubscriber()

#     rclpy.spin(minimal_subscriber)

#     # Destroy the node explicitly
#     # (optional - otherwise it will be done automatically
#     # when the garbage collector destroys the node object)
#     minimal_subscriber.destroy_node()
#     rclpy.shutdown()


# if __name__ == '__main__':
#     main()

# def init()


# # Control table address
# ADDR_TORQUE_ENABLE      = 64               # Control table address is different in Dynamixel model
# ADDR_GOAL_POSITION      = 116
# ADDR_PRESENT_POSITION   = 132

# # Protocol version
# PROTOCOL_VERSION            = 2.0               # See which protocol version is used in the Dynamixel

# # Default setting
# DXL_ID                      = 1                 # Dynamixel ID : 1
# BAUDRATE                    = 57600             # Dynamixel default baudrate : 57600
# DEVICENAME                  = '/dev/ttyUSB0'    # Check which port is being used on your controller
#                                                 # ex) Windows: "COM1"   Linux: "/dev/ttyUSB0" Mac: "/dev/tty.usbserial-*"

# TORQUE_ENABLE               = 1                 # Value for enabling the torque
# TORQUE_DISABLE              = 0                 # Value for disabling the torque
# DXL_MINIMUM_POSITION_VALUE  = 0               # Dynamixel will rotate between this value
# DXL_MAXIMUM_POSITION_VALUE  = 1000            # and this value (note that the Dynamixel would not move when the position value is out of movable range. Check e-manual about the range of the Dynamixel you use.)
# DXL_MOVING_STATUS_THRESHOLD = 20                # Dynamixel moving status threshold

# portHandler = PortHandler(DEVICENAME)
# packetHandler = PacketHandler(PROTOCOL_VERSION)

# def set_goal_pos_callback(data):
#     print("Set Goal Position of ID %s = %s" % (data.id, data.position))
#     dxl_comm_result, dxl_error = packetHandler.write4ByteTxRx(portHandler, data.id, ADDR_GOAL_POSITION, data.position)

# def get_present_pos(req):
#     dxl_present_position, dxl_comm_result, dxl_error = packetHandler.read4ByteTxRx(portHandler, req.id, ADDR_PRESENT_POSITION)
#     print("Present Position of ID %s = %s" % (req.id, dxl_present_position))
#     return dxl_present_position

# def read_write_py_node():
#     rospy.init_node('read_write_py_node')
#     rospy.Subscriber('set_position', SetPosition, set_goal_pos_callback)
#     rospy.Service('get_position', GetPosition, get_present_pos)
#     rospy.spin()

# def main():
#     # Open port
#     try:
#        portHandler.openPort()
#        print("Succeeded to open the port")
#     except:
#         print("Failed to open the port")
#         print("Press any key to terminate...")
#         getch()
#         quit()

#     # Set port baudrate
#     try:
#         portHandler.setBaudRate(BAUDRATE)
#         print("Succeeded to change the baudrate")
#     except:
#         print("Failed to change the baudrate")
#         print("Press any key to terminate...")
#         getch()
#         quit()

#     # Enable Dynamixel Torque
#     dxl_comm_result, dxl_error = packetHandler.write1ByteTxRx(portHandler, DXL_ID, ADDR_TORQUE_ENABLE, TORQUE_ENABLE)
#     if dxl_comm_result != COMM_SUCCESS:
#         print("%s" % packetHandler.getTxRxResult(dxl_comm_result))
#         print("Press any key to terminate...")
#         getch()
#         quit()
#     elif dxl_error != 0:
#         print("%s" % packetHandler.getRxPacketError(dxl_error))
#         print("Press any key to terminate...")
#         getch()
#         quit()
#     else:
#         print("DYNAMIXEL has been successfully connected")

#     print("Ready to get & set Position.")

#     read_write_py_node()


# if __name__ == '__main__':
#     main()
