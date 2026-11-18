#include <cstdio>
#include <iostream>

#include "yatrices/vector.hpp"

using namespace yatrices;

static const char* yes_no(bool b) { return b ? "yes" : "no"; }

int main() {
    std::puts("1. Plain doubles");
    double sum = 0.1 + 0.2;
    std::printf("   0.1 + 0.2 = %.17g\n", sum);
    std::printf("   0.3       = %.17g\n", 0.3);
    std::printf("   exactly equal (==)?  %s\n", yes_no(sum == 0.3));
    std::printf("   difference           = %.3g (one ulp)\n", sum - 0.3);
    std::printf("   approx_equal?        %s\n\n", yes_no(approx_equal(sum, 0.3)));

    std::puts("2. The same sum with Vec<double>");
    Vecd a{0.1};
    Vecd b{0.2};
    a.axpy(1.0, b);
    Vecd expected{0.3};
    std::printf("   a[0]        = %.17g\n", a[0]);
    std::printf("   expected[0] = %.17g\n", expected[0]);
    std::printf("   a[0] == expected[0]? %s\n", yes_no(a[0] == expected[0]));
    std::printf("   allclose(a, expected)? %s\n", yes_no(allclose(a, expected)));
    std::cout << "   printed: " << a << "   all 17 digits: " << to_string(a, 17) << "\n\n";

    std::puts("3. The same sum with Vec<float>");
    Vecf fa{0.1f};
    Vecf fb{0.2f};
    fa.axpy(1.0f, fb);
    std::printf("   0.1f + 0.2f = %.9g\n", fa[0]);
    std::printf("   0.3f        = %.9g\n", 0.3f);
    std::printf("   exactly equal (==)?  %s\n", yes_no(fa[0] == 0.3f));
    return 0;
}
