// Example: why 0.1 + 0.2 != 0.3, and how to compare floats safely.
//
// Build and run from the project folder:
//   cmake -S . -B build && cmake --build build
//   ./build/cpp/examples/example_float_sum

#include <cstdio>
#include <iostream>

#include "yatrices/vector.hpp"

// Everything in the library lives in the namespace yatrices, so its names
// cannot clash with yours. This line lets the example write Vecd instead of
// yatrices::Vecd. Fine in a small program; avoid it in headers.
using namespace yatrices;

static const char* yes_no(bool b) { return b ? "yes" : "no"; }

int main() {
    std::puts("1. Plain doubles");
    double sum = 0.1 + 0.2;
    // %.17g prints 17 significant digits: enough to see every digit of a double.
    std::printf("   0.1 + 0.2 = %.17g\n", sum);
    std::printf("   0.3       = %.17g\n", 0.3);
    std::printf("   exactly equal (==)?  %s\n", yes_no(sum == 0.3));
    std::printf("   difference           = %.3g (one ulp)\n", sum - 0.3);
    std::printf("   approx_equal?        %s\n\n", yes_no(approx_equal(sum, 0.3)));

    // axpy(alpha, x) does this += alpha * x, so a becomes 0.1 + 1.0 * 0.2.
    std::puts("2. The same sum with Vec<double>");
    Vecd a{0.1};
    Vecd b{0.2};
    a.axpy(1.0, b);
    Vecd expected{0.3};
    std::printf("   a[0]        = %.17g\n", a[0]);
    std::printf("   expected[0] = %.17g\n", expected[0]);
    std::printf("   a[0] == expected[0]? %s\n", yes_no(a[0] == expected[0]));
    std::printf("   allclose(a, expected)? %s\n", yes_no(allclose(a, expected)));
    // Printing rounds to 15 digits; the stored value is NOT changed.
    std::cout << "   printed: " << a << "   all 17 digits: " << to_string(a, 17) << "\n\n";

    // float keeps about 7 digits, so its error is much larger (about 1e-8),
    // but here the rounding happens to land on the same number as 0.3f.
    std::puts("3. The same sum with Vec<float>");
    Vecf fa{0.1f};
    Vecf fb{0.2f};
    fa.axpy(1.0f, fb);
    std::printf("   0.1f + 0.2f = %.9g\n", fa[0]);
    std::printf("   0.3f        = %.9g\n", 0.3f);
    std::printf("   exactly equal (==)?  %s\n", yes_no(fa[0] == 0.3f));
    return 0;
}
