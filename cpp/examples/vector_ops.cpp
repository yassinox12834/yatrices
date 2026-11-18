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
    std::cout << "   x + y    = " << x + y << "\n";
    std::cout << "   y - x    = " << y - x << "\n";
    std::cout << "   -x       = " << -x << "\n";
    std::cout << "   2 * x  = " << 2 * x << "\n";
    std::cout << "   y / 2  = " << y / 2 << "\n";

    Vecd z = x;
    z += y;
    z *= 10;
    std::cout << "   z = x; z += y; z *= 10  ->  z = " << z << "\n\n";

    std::puts("2. Products");
    std::printf("   x . y       = %g\n", x.dot(y));
    std::cout << "   hadamard    = " << hadamard(x, y) << "\n";
    std::cout << "   cross(x, y) = " << cross(x, y) << "\n\n";

    Vecd v{3.0, -4.0};
    std::cout << "3. Norms and reductions of v = " << v << "\n";
    std::printf("   norm1    = %g\n", v.norm1());
    std::printf("   norm2    = %g\n", v.norm2());
    std::printf("   norm_inf = %g\n", v.norm_inf());
    std::printf("   sum = %g, mean = %g, min = %g, max = %g\n",
                v.sum(), v.mean(), v.min(), v.max());

    Vecd huge{3e200, 4e200};
    std::printf("   norm2 of [3e200, 4e200] = %g (overflow-safe)\n\n", huge.norm2());

    std::puts("4. Geometry");
    Vecd e1{1.0, 0.0};
    Vecd d{1.0, 1.0};
    std::printf("   angle(e1, d) = %.6f rad = %.1f degrees\n",
                angle(e1, d), angle(e1, d) * 180.0 / 3.141592653589793);
    Vecd p{2.0, 3.0};
    std::cout << "   project([2, 3] onto [1, 0]) = " << project(p, e1) << "\n\n";

    std::puts("5. Errors");
    try {
        Vecd short_one{1.0, 2.0};
        Vecd bad = x + short_one;
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
        (void)angle(e1, Vecd(2));
    } catch (const ZeroVectorError& e) {
        std::printf("   %s\n", e.what());
    }
    return 0;
}
