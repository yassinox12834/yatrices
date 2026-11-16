// Example: everything a Vec can do, on small vectors you can check by hand.
//
// Build and run from the project folder:
//   cmake -S . -B build && cmake --build build
//   ./build/cpp/examples/example_vector_ops

#include <cstdio>
#include <iostream>

#include "yatrices/errors.hpp"
#include "yatrices/vector.hpp"

using namespace yatrices;

int main() {
    Vecd x{1, 2, 3};
    Vecd y{4, 5, 6};
    std::cout << "x = " << x << "\ny = " << y << "\n\n";

    std::puts("1. Arithmetic (x and y are never changed)");
    std::cout << "   x + y    = " << x + y << "\n";       // [5, 7, 9]
    std::cout << "   y - x    = " << y - x << "\n";       // [3, 3, 3]
    std::cout << "   -x       = " << -x << "\n";          // [-1, -2, -3]
    std::cout << "   2 * x  = " << 2 * x << "\n";     // [2, 4, 6]
    std::cout << "   y / 2  = " << y / 2 << "\n";     // [2, 2.5, 3]

    Vecd z = x;                   // a copy, so x stays [1, 2, 3]
    z += y;                       // z = [5, 7, 9]
    z *= 10;                    // z = [50, 70, 90]
    std::cout << "   z = x; z += y; z *= 10  ->  z = " << z << "\n\n";

    std::puts("2. Products");
    std::printf("   x . y       = %g\n", x.dot(y));       // 4 + 10 + 18 = 32
    std::cout << "   hadamard    = " << hadamard(x, y) << "\n";  // [4, 10, 18]
    std::cout << "   cross(x, y) = " << cross(x, y) << "\n\n";   // [-3, 6, -3]

    Vecd v{3.0, -4.0};
    std::cout << "3. Norms and reductions of v = " << v << "\n";
    std::printf("   norm1    = %g\n", v.norm1());          // 3 + 4 = 7
    std::printf("   norm2    = %g\n", v.norm2());          // sqrt(9 + 16) = 5
    std::printf("   norm_inf = %g\n", v.norm_inf());       // max(3, 4) = 4
    std::printf("   sum = %g, mean = %g, min = %g, max = %g\n",
                v.sum(), v.mean(), v.min(), v.max());  // -1, -0.5, -4, 3

    Vecd huge{3e200, 4e200};      // squaring 3e200 would overflow to infinity
    std::printf("   norm2 of [3e200, 4e200] = %g (overflow-safe)\n\n", huge.norm2());

    std::puts("4. Geometry");
    Vecd e1{1.0, 0.0};
    Vecd d{1.0, 1.0};
    std::printf("   angle(e1, d) = %.6f rad = %.1f degrees\n",
                angle(e1, d), angle(e1, d) * 180.0 / 3.141592653589793);  // pi/4, 45
    Vecd p{2.0, 3.0};
    std::cout << "   project([2, 3] onto [1, 0]) = " << project(p, e1) << "\n\n";  // [2, 0]

    std::puts("5. Errors");
    try {
        Vecd short_one{1.0, 2.0};
        Vecd bad = x + short_one;     // sizes 3 and 2
    } catch (const SizeMismatchError& e) {
        std::printf("   %s\n", e.what());
    }
    try {
        Vecd empty;
        (void)empty.max();
    } catch (const EmptyVectorError& e) {
        std::printf("   %s\n", e.what());
    }
    try {
        (void)angle(e1, Vecd(2));     // Vecd(2) is [0, 0]
    } catch (const ZeroVectorError& e) {
        std::printf("   %s\n", e.what());
    }
    return 0;
}
