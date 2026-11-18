"""A short tour of yatricespy.Vec. Run it with: python python/examples/vector_tour.py"""
import math

import yatricespy as yt

x = yt.Vec([1, 2, 3])
y = yt.Vec([4, 5, 6])
print("x =", x, " y =", y)

print("x + y =", x + y)
print("2 * x =", 2 * x)
print("y / 2 =", y / 2)

print("x . y =", x @ y)
print("hadamard =", yt.hadamard(x, y))
print("cross =", yt.cross(x, y))

v = yt.Vec([3, -4])
print("norm2 =", v.norm2(), " mean =", v.mean())

print(f"angle = {math.degrees(yt.angle(yt.Vec([1, 0]), yt.Vec([1, 1]))):.1f} degrees")

print("0.1 + 0.2 == 0.3 ?", 0.1 + 0.2 == 0.3)
print("approx_equal ?", yt.approx_equal(0.1 + 0.2, 0.3))

try:
    x + yt.Vec([1, 2])
except yt.SizeMismatchError as e:
    print("Error:", e)
