from typing import Union
import numpy as np
import math

M_PI = math.pi
M_PI_2 = M_PI / 2
M_PI_3 = M_PI / 3
M_PI_4 = M_PI / 4


def vector2d(x: Union[int, float], y: Union[int, float]):
    """
    Returns a 2D vector as a numpy array.

    Params:
        x [Union[int, float]]: X component of the vector
        y [Union[int, float]]: Y component of the vector
    """
    return np.array([x, y])


def rotation2d(theta: Union[int, float]):
    """
    Return a 2D rotation matrix as a numpy array, provided an angle in radians.
    From https://scipython.com/book/chapter-6-numpy/examples/creating-a-rotation-matrix-in-numpy/

    Positive rotations move vectors clockwise, whereas negative rotations move
    vectors counterclockwise

    Params:
        theta [Union[int, float]]: Angle in radians
    """
    c, s = np.cos(theta), np.sin(theta)
    return np.array(((c, -s), (s, c)))


def smallest_angle(theta: Union[int, float]):
    """
    Given an angle in radians, returns the smallest rotation for the angle,
    between -PI and PI

    Params:
        theta [Union[int, float]]: Angle in radians
    """
    # Reduce the angle down to 0-PI
    reduced_theta = abs(theta) % (M_PI)

    # If the value is between pi and 2pi (modulus 2pi to not count multiple
    # rotations), we need to use 360 minus the angle's value
    opposite_hemisphere = (abs(theta) // M_PI) % 2 != 0

    if opposite_hemisphere:
        # If the shortest angle is in the opposite hemisphere, we reverse
        # the angle and take the opposite of the angle between 0-180.
        # Ex. For 210deg, the shortest angle is actually -150deg, whereas
        # if directly flipped, it would be reported as -30deg
        return math.copysign(M_PI - reduced_theta, -theta)
    else:
        # If we are in the same hemisphere, we can just remove any extra
        # rotations and keep the same sign.
        return math.copysign(reduced_theta, theta)
