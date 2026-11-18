#include <cstdio>
#include <iostream>

#include "yatrices/yatrices.hpp"

using namespace yatrices;

int main() {
    Matrixd A{{1, 2, 3},
              {4, 5, 6}};
    std::cout << "A =\n" << A << "\nshape " << A.rows() << " x " << A.cols() << "\n\n";

    std::puts("1. Arithmetic, entry by entry (A is never changed)");
    std::cout << "A + A =\n" << A + A << "\n";
    std::cout << "A / 2 =\n" << A / 2 << "\n";
    std::cout << "A o A (hadamard) =\n" << hadamard(A, A) << "\n\n";

    std::puts("2. Transpose: row i becomes column i");
    std::cout << "A^T =\n" << A.transposed() << "\n\n";

    std::puts("3. Shape changes without moving any number");
    Matrixd R = A;
    R.reshape(3, 2);
    std::cout << "A reshaped to 3 x 2 =\n" << R << "\n";
    std::cout << "flatten(A) = " << A.flatten() << "\n\n";

    std::puts("4. Reductions");
    std::cout << "sum = " << A.sum() << ", mean = " << A.mean() << "\n";
    std::cout << "sum_rows = " << A.sum_rows() << "   (one per row)\n";
    std::cout << "sum_cols = " << A.sum_cols() << "   (one per column)\n\n";

    std::puts("5. Square matrices: trace and diagonals");
    Matrixd Q{{2, 1, 0},
              {1, 3, 1},
              {0, 1, 4}};
    std::cout << "trace(Q) = " << Q.trace() << "\n";
    std::cout << "diagonal(0) = " << Q.diagonal() << ", diagonal(1) = " << Q.diagonal(1)
              << "\n\n";

    std::puts("6. One step of Gaussian elimination with row operations");
    Matrixd G{{2, 1, -1},
              {-3, -1, 2},
              {-2, 1, 2}};
    G.add_row_multiple(1, 0, 1.5);
    G.add_row_multiple(2, 0, 1.0);
    std::cout << G << "\n\n";

    std::puts("7. Centring the columns (the first step of PCA)");
    Matrixd X{{1, 10},
              {2, 20},
              {3, 30}};
    Vecd means = centre_columns(X);
    std::cout << "means = " << means << "\nX centred =\n" << X << "\n\n";

    std::puts("8. Errors say what went wrong");
    try {
        (void)(A + Q);
    } catch (const ShapeMismatchError& e) {
        std::cout << e.what() << "\n";
    }
    try {
        (void)A.trace();
    } catch (const NotSquareError& e) {
        std::cout << e.what() << "\n";
    }
    return 0;
}
