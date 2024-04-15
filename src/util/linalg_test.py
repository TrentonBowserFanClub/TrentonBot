import unittest
import math
from src.util.linalg import (
    vector2d,
    rotation2d,
    smallest_angle,
    M_PI,
    M_PI_2,
    M_PI_3,
    M_PI_4,
)


class TestVector2D(unittest.TestCase):
    def test_add(self):
        total = vector2d(1, 2) + vector2d(3, 4)
        self.assertEqual(total[0], 4)
        self.assertEqual(total[1], 6)

    def test_iadd(self):
        vec_a = vector2d(1, 2)
        vec_a += vector2d(5, 5)
        self.assertEqual(vec_a[0], 6)
        self.assertEqual(vec_a[1], 7)

    def test_sub(self):
        total = vector2d(5, 5) - vector2d(3, 4)
        self.assertEqual(total[0], 2)
        self.assertEqual(total[1], 1)

    def test_isub(self):
        vec_a = vector2d(5, 5)
        vec_a -= vector2d(1, 2)
        self.assertEqual(vec_a[0], 4)
        self.assertEqual(vec_a[1], 3)

    def test_scalar_multiply(self):
        total = vector2d(1, 2) * 3
        self.assertEqual(total[0], 3)
        self.assertEqual(total[1], 6)

    def test_scalar_divide(self):
        total = vector2d(8, 4) / 2
        self.assertEqual(total[0], 4)
        self.assertEqual(total[1], 2)

    def test_45deg_rotation(self):
        angle = M_PI_4
        starting_vec = vector2d(1, 2)

        # Figure out the expected angles programmatically
        r = math.sqrt(5)
        expected_angle_cw = math.atan2(starting_vec[1], starting_vec[0]) - M_PI_4
        expected_vec_cw = vector2d(
            r * math.cos(expected_angle_cw), r * math.sin(expected_angle_cw)
        )

        expected_angle_ccw = math.atan2(starting_vec[1], starting_vec[0]) + M_PI_4
        expected_vec_ccw = vector2d(
            r * math.cos(expected_angle_ccw), r * math.sin(expected_angle_ccw)
        )

        for theta, expected in [(angle, expected_vec_cw), (-angle, expected_vec_ccw)]:
            rotation = rotation2d(theta)

            rotated = starting_vec.dot(rotation)
            self.assertEqual(rotated.shape, (2,))
            self.assertAlmostEqual(rotated[0], expected[0])
            self.assertAlmostEqual(rotated[1], expected[1])

    def test_90deg_rotation(self):
        angle = M_PI_2
        starting_vec = vector2d(1, 2)
        expected_vec = vector2d(2, -1)

        for theta, expected in [(angle, expected_vec), (-angle, -expected_vec)]:
            rotation = rotation2d(theta)

            rotated = starting_vec.dot(rotation)
            self.assertEqual(rotated.shape, (2,))
            self.assertAlmostEqual(rotated[0], expected[0])
            self.assertAlmostEqual(rotated[1], expected[1])

    def test_180deg_rotation(self):
        angle = M_PI
        starting_vec = vector2d(1, 2)
        expected_vec = vector2d(-1, -2)

        for theta, expected in [(angle, expected_vec), (-angle, expected_vec)]:
            rotation = rotation2d(theta)

            rotated = starting_vec.dot(rotation)
            self.assertEqual(rotated.shape, (2,))
            self.assertAlmostEqual(rotated[0], expected[0])
            self.assertAlmostEqual(rotated[1], expected[1])

    def test_270deg_rotation(self):
        angle = 3 * M_PI_2
        starting_vec = vector2d(1, 2)
        expected_vec = vector2d(-2, 1)

        for theta, expected in [(angle, expected_vec), (-angle, -expected_vec)]:
            rotation = rotation2d(theta)

            rotated = starting_vec.dot(rotation)
            self.assertEqual(rotated.shape, (2,))
            self.assertAlmostEqual(rotated[0], expected[0])
            self.assertAlmostEqual(rotated[1], expected[1])

    def test_360deg_rotation(self):
        angle = 2 * M_PI
        starting_vec = vector2d(1, 2)
        expected_vec = vector2d(1, 2)

        for theta, expected in [(angle, expected_vec), (-angle, expected_vec)]:
            rotation = rotation2d(theta)

            rotated = starting_vec.dot(rotation)
            self.assertEqual(rotated.shape, (2,))
            self.assertAlmostEqual(rotated[0], expected[0])
            self.assertAlmostEqual(rotated[1], expected[1])


class TestRotation2D(unittest.TestCase):
    def test_add_90deg_90deg(self):
        """
        Test that adding two 90 degree rotations adds to a
        180 degree rotation.
        """
        rot_a = rotation2d(M_PI_2)
        rot_b = rotation2d(M_PI_2)
        expected_total = rotation2d(M_PI)

        total = rot_a.dot(rot_b)
        self.assertTrue((total == expected_total).all())

    def test_add_neg90deg_neg90deg(self):
        """
        Test that adding two -90 degree rotations adds to a
        -180 degree rotation.
        """
        rot_a = rotation2d(-M_PI_2)
        rot_b = rotation2d(-M_PI_2)
        expected_total = rotation2d(-M_PI)

        total = rot_a.dot(rot_b)
        self.assertTrue((total == expected_total).all())

    def test_add_90deg_180deg(self):
        """
        Test that adding a 90 and 180 degree rotation adds to
        a 270 degree rotation
        """
        rot_a = rotation2d(M_PI_2)
        rot_b = rotation2d(M_PI)
        expected_total = rotation2d(3 * M_PI_2)

        total = rot_a.dot(rot_b)
        self.assertTrue((total == expected_total).all())


class TestSmallestAngle(unittest.TestCase):
    def test_small_positive_angle(self):
        """
        Test a small, positive angle (between 0 and PI) to confirm the
        same angle gets returned
        """
        angle = smallest_angle(M_PI_4)
        self.assertAlmostEqual(angle, M_PI_4)

    def test_small_negative_angle(self):
        """
        Test a small, negative angle (between 0 and -PI) to confirm the
        same angle gets returned
        """
        angle = smallest_angle(-M_PI_4)
        self.assertAlmostEqual(angle, -M_PI_4)

    def test_mid_positive_angle(self):
        """
        Test a medium, positive angle (between PI and 2PI) to confirm that
        the smaller negative angle is returned
        """
        angle = smallest_angle(5 * M_PI_3)
        self.assertAlmostEqual(angle, -M_PI_3)

    def test_mid_negative_angle(self):
        """
        Test a medium, negative angle (between -PI and -2PI) to confirm that
        the smaller positive angle is returned
        """
        angle = smallest_angle(-5 * M_PI_3)
        self.assertAlmostEqual(angle, M_PI_3)

    def test_large_positive_angle(self):
        """
        Test a large, positive angle (between 2PI and 3PI) to confirm
        that the same positive angle is returned
        """
        angle = smallest_angle(5 * M_PI_2)
        self.assertAlmostEqual(angle, M_PI_2)

    def test_large_negative_angle(self):
        """
        Test a large, negative angle (between -2PI and -3PI) to confirm
        that the same negative angle is returned
        """
        angle = smallest_angle(-5 * M_PI_2)
        self.assertAlmostEqual(angle, -M_PI_2)

    def test_multi_rollover_positive_angle(self):
        """
        Test a large, positive angle (0 and PI, + n*2PI) to confirm
        that the reduced positive angle is returned
        """
        angle = smallest_angle(M_PI_3 + (14 * 2 * M_PI))
        self.assertAlmostEqual(angle, M_PI_3)

    def test_multi_rollover_negative_angle(self):
        """
        Test a large, negative angle (0 and -PI, + n*-2PI) to confirm
        that the reduced negative angle is returned
        """
        angle = smallest_angle(-M_PI_3 + (14 * -2 * M_PI))
        self.assertAlmostEqual(angle, -M_PI_3)


if __name__ == "__main__":
    unittest.main()
