"""A short tour of yatricespy.Vec. Run it with: python python/examples/vector_tour.py"""
import math

import yatricespy as yt

x = yt.Vec([1, 2, 3])
y = yt.Vec([4, 5, 6])
print("x =", x, " y =", y)

# 1. Arithmetic: a new vector each time, x and y are untouched
print("x + y =", x + y)        # [5, 7, 9]
print("2 * x =", 2 * x)        # [2, 4, 6]
print("y / 2 =", y / 2)        # [2, 2.5, 3]

# 2. Products
print("x . y =", x @ y)                    # 32
print("hadamard =", yt.hadamard(x, y))     # [4, 10, 18]
print("cross =", yt.cross(x, y))           # [-3, 6, -3]

# 3. Norms and reductions
v = yt.Vec([3, -4])
print("norm2 =", v.norm2(), " mean =", v.mean())   # 5.0, -0.5

# 4. Geometry
print(f"angle = {math.degrees(yt.angle(yt.Vec([1, 0]), yt.Vec([1, 1]))):.1f} degrees")  # 45.0

# 5. Floats: never compare them with ==
print("0.1 + 0.2 == 0.3 ?", 0.1 + 0.2 == 0.3)                       # False
print("approx_equal ?", yt.approx_equal(0.1 + 0.2, 0.3))            # True

# 6. Errors say what went wrong and what to do
try:
    x + yt.Vec([1, 2])
except yt.SizeMismatchError as e:
    print("Error:", e)
