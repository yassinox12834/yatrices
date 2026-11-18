#include <cmath>
#include <cstdint>
#include <cstdio>
#include <limits>
#include <sstream>
#include <string>

#include "yatrices/yatrices.hpp"

using namespace yatrices;

namespace {

int g_failures = 0;
int g_checks = 0;

#define CHECK(...)                                                               \
    do {                                                                         \
        ++g_checks;                                                              \
        if (!(__VA_ARGS__)) {                                                    \
            ++g_failures;                                                        \
            std::printf("FAILED %s:%d: %s\n", __FILE__, __LINE__, #__VA_ARGS__); \
        }                                                                        \
    } while (0)

#define CHECK_THROWS(expr, Error)                                                \
    do {                                                                         \
        ++g_checks;                                                              \
        bool thrown = false;                                                     \
        try {                                                                    \
            (void)(expr);                                                        \
        } catch (const Error&) {                                                 \
            thrown = true;                                                       \
        } catch (...) {                                                          \
        }                                                                        \
        if (!thrown) {                                                           \
            ++g_failures;                                                        \
            std::printf("FAILED %s:%d: %s did not throw %s\n", __FILE__, __LINE__, \
                        #expr, #Error);                                          \
        }                                                                        \
    } while (0)

template <typename T>
bool close(T a, T b) {
    return approx_equal(a, b);
}

template <typename T>
void test_vec_new_features() {
    using V = Vec<T>;

    CHECK((V::ones(3) == V{1, 1, 1}));
    CHECK((V::unit(4, 2) == V{0, 0, 1, 0}));
    CHECK_THROWS(V::unit(3, 3), IndexError);
    CHECK((V::linspace(0, 1, 5) == V{0, 0.25, 0.5, 0.75, 1}));
    CHECK(V::linspace(2, 7, 1).size() == 1 && V::linspace(2, 7, 0).empty());
    CHECK(V::linspace(T(0.1), T(0.7), 7)[6] == T(0.7));

    V v{3, -1, 4, -1, 5};
    CHECK(v.argmin() == 1);
    CHECK(v.argmax() == 4);
    CHECK_THROWS(V().argmax(), EmptyVectorError);
    CHECK(close(v.prod(), T(60)));
    CHECK(V().prod() == T(1));

    V w{2, 4, 4, 4, 5, 5, 7, 9};
    CHECK(close(w.variance(false), T(4)));
    CHECK(close(w.variance(true), T(32) / T(7)));
    CHECK(close(w.stddev(false), T(2)));
    CHECK_THROWS(V{1}.variance(true), WrongSizeError);
    CHECK(close(V{1}.variance(false), T(0)));

    V n{3, 4};
    CHECK(allclose(n.normalised(), V{0.6, 0.8}));
    n.normalise();
    CHECK(close(n.norm2(), T(1)));
    CHECK_THROWS(V(3).normalise(), ZeroVectorError);

    CHECK((V{0, 1e-9, -2, 0}.count_nonzero() == 2));
    CHECK((V{0, 1e-9, -2, 0}.count_nonzero(T(1e-6)) == 1));
    CHECK(!V{1, 2}.has_nan());
    CHECK((V{1, std::numeric_limits<T>::quiet_NaN()}.has_nan()));

    V a{1, 2, 3}, b{4, 6, 3};
    CHECK((lincomb(2, a, -1, b) == V{-2, -2, 3}));
    CHECK((lerp(a, b, 0) == a) && (lerp(a, b, 1) == b));
    CHECK(allclose(lerp(a, b, 0.5), V{2.5, 4, 3}));
    CHECK(close(distance(a, b), T(5)));
    CHECK(close(distance1(a, b), T(7)));
    CHECK(close(distance_inf(a, b), T(4)));
    CHECK(distance(a, a) == T(0));
    CHECK_THROWS(distance(a, V(2)), SizeMismatchError);

    const T big = std::numeric_limits<T>::max() / 4;
    CHECK(close(distance(V{big, 0}, V{0, 0}), big));
    const T tiny = std::numeric_limits<T>::min() * 4;
    CHECK(close(distance(V{tiny, 0}, V{0, 0}), tiny));

    CHECK(close(cosine_similarity(V{1, 0}, V{0, 5}), T(0)));
    CHECK(close(cosine_similarity(V{1, 1}, V{2, 2}), T(1)));
    CHECK_THROWS(cosine_similarity(V{0, 0}, V{1, 1}), ZeroVectorError);

    V sq{1, 2, 3};
    sq.apply([](T x) { return x * x; });
    CHECK((sq == V{1, 4, 9}));
    CHECK((V{1, 2} != V{1, 3}) && (V{1, 2} != V{1, 2, 3}));
}

template <typename T>
void test_matrix_construction() {
    using M = Matrix<T>;

    M z(2, 3);
    CHECK(z.rows() == 2 && z.cols() == 3 && z.size() == 6);
    CHECK(z.sum() == T(0) && !z.empty() && !z.is_square());
    CHECK(M(2, 2, 7).sum() == T(28));
    CHECK(M().empty() && M(0, 5).empty() && M(5, 0).empty());

    M A{{1, 2, 3},
        {4, 5, 6}};
    CHECK(A(0, 0) == 1 && A(0, 2) == 3 && A(1, 0) == 4 && A(1, 2) == 6);
    CHECK(A.data()[5] == 6);
    CHECK(A.row_data(1)[0] == 4);
    CHECK_THROWS((M{{1, 2}, {3}}), RaggedRowsError);

    CHECK(A.at(1, 2) == 6);
    CHECK_THROWS(A.at(2, 0), IndexError);
    CHECK_THROWS(A.at(0, 3), IndexError);

    CHECK(reinterpret_cast<std::uintptr_t>(M(3, 5).data()) % 64 == 0);

    M B = A;
    B(0, 0) = 100;
    CHECK(A(0, 0) == 1);
    M C = std::move(B);
    CHECK(C(0, 0) == 100 && B.data() == nullptr && B.rows() == 0);
    M D(3, 2);
    const T* before = D.data();
    D = A;
    CHECK(D.data() == before && D.rows() == 2 && D.cols() == 3 && allclose(D, A));
    M E(1, 1);
    E = A;
    CHECK(allclose(E, A));
    E = E;
    CHECK(allclose(E, A));

    M R = A;
    R.reshape(3, 2);
    CHECK(R.rows() == 3 && R(2, 1) == 6 && R(1, 0) == 3);
    CHECK_THROWS(R.reshape(4, 2), ShapeMismatchError);
    CHECK((A.flatten() == Vec<T>{1, 2, 3, 4, 5, 6}));
}

template <typename T>
void test_matrix_arithmetic() {
    using M = Matrix<T>;
    M A{{1, 2}, {3, 4}};
    M B{{10, 20}, {30, 40}};

    CHECK(allclose(A + B, M{{11, 22}, {33, 44}}));
    CHECK(allclose(B - A, M{{9, 18}, {27, 36}}));
    CHECK(allclose(-A, M{{-1, -2}, {-3, -4}}));
    CHECK(allclose(2 * A, M{{2, 4}, {6, 8}}));
    CHECK(allclose(A * 0.5, M{{0.5, 1}, {1.5, 2}}));
    CHECK(allclose(B / 10, M{{1, 2}, {3, 4}}));
    CHECK(allclose(A + B + A, M{{12, 24}, {36, 48}}));
    CHECK(allclose(hadamard(A, B), M{{10, 40}, {90, 160}}));
    CHECK_THROWS(A + M(2, 3), ShapeMismatchError);
    CHECK_THROWS(hadamard(A, M(3, 2)), ShapeMismatchError);

    M C = A;
    C += B;
    C -= A;
    CHECK(allclose(C, B));
    C *= 2;
    C /= 4;
    CHECK(allclose(C, M{{5, 10}, {15, 20}}));
    C += C;
    CHECK(allclose(C, M{{10, 20}, {30, 40}}));
    C -= C;
    CHECK(C.sum() == T(0));

    M F(2, 2);
    F.fill(3);
    F.apply([](T x) { return x * x; });
    CHECK(F.sum() == T(36));
}

template <typename T>
void test_matrix_transpose_and_diagonals() {
    using M = Matrix<T>;
    M A{{1, 2, 3},
        {4, 5, 6}};
    M At = A.transposed();
    CHECK(At.rows() == 3 && At.cols() == 2);
    CHECK(allclose(At, M{{1, 4}, {2, 5}, {3, 6}}));
    CHECK(allclose(At.transposed(), A));

    M L(70, 45, uninitialised);
    for (std::size_t i = 0; i < L.rows(); ++i)
        for (std::size_t j = 0; j < L.cols(); ++j) L(i, j) = T(i * 100 + j);
    M Lt = L.transposed();
    bool ok = true;
    for (std::size_t i = 0; i < L.rows(); ++i)
        for (std::size_t j = 0; j < L.cols(); ++j) ok = ok && Lt(j, i) == L(i, j);
    CHECK(ok);

    M S(67, 67, uninitialised);
    for (std::size_t k = 0; k < S.size(); ++k) S.data()[k] = T(k);
    M St = S.transposed();
    S.transpose_in_place();
    CHECK(allclose(S, St));
    CHECK_THROWS(A.transpose_in_place(), NotSquareError);

    M Q{{1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}};
    CHECK(Q.trace() == T(15));
    CHECK_THROWS(A.trace(), NotSquareError);
    CHECK((Q.diagonal() == Vec<T>{1, 5, 9}));
    CHECK((Q.diagonal(1) == Vec<T>{2, 6}));
    CHECK((Q.diagonal(-1) == Vec<T>{4, 8}));
    CHECK((Q.diagonal(2) == Vec<T>{3}));
    CHECK(Q.diagonal(3).empty() && Q.diagonal(-7).empty());
    CHECK((A.diagonal() == Vec<T>{1, 5}));
    CHECK((A.diagonal(-1) == Vec<T>{4}));
    Q.set_diagonal(Vec<T>{0, 0, 0});
    CHECK(Q.trace() == T(0));
    Q.set_diagonal(Vec<T>{-1, -2}, -1);
    CHECK(Q(1, 0) == T(-1) && Q(2, 1) == T(-2));
    CHECK_THROWS(Q.set_diagonal(Vec<T>{1, 2}), WrongSizeError);
}

template <typename T>
void test_matrix_rows_and_reductions() {
    using M = Matrix<T>;
    M A{{1, 2, 3},
        {4, 5, 6}};

    M B = A;
    B.swap_rows(0, 1);
    CHECK(allclose(B, M{{4, 5, 6}, {1, 2, 3}}));
    B.scale_row(0, 0.5);
    CHECK(allclose(B, M{{2, 2.5, 3}, {1, 2, 3}}));
    B.add_row_multiple(1, 0, -0.5);
    CHECK(allclose(B, M{{2, 2.5, 3}, {0, 0.75, 1.5}}));
    B.add_row_multiple(0, 0, 1);
    CHECK(allclose(B, M{{4, 5, 6}, {0, 0.75, 1.5}}));

    CHECK(A.sum() == T(21) && close(A.mean(), T(3.5)));
    CHECK(A.min() == T(1) && A.max() == T(6));
    CHECK((M{{-7, 2}, {3, 4}}.max_abs() == T(7)));
    CHECK_THROWS(M().mean(), EmptyMatrixError);
    CHECK_THROWS(M(0, 3).max(), EmptyMatrixError);
    CHECK(A.all_finite());

    CHECK((A.sum_rows() == Vec<T>{6, 15}));
    CHECK((A.sum_cols() == Vec<T>{5, 7, 9}));
    CHECK(allclose(A.mean_rows(), Vec<T>{2, 5}));
    CHECK(allclose(A.mean_cols(), Vec<T>{2.5, 3.5, 4.5}));
    CHECK_THROWS(M(0, 3).mean_cols(), EmptyMatrixError);

    M C = A;
    add_to_rows(C, Vec<T>{10, 20, 30});
    CHECK(allclose(C, M{{11, 22, 33}, {14, 25, 36}}));
    subtract_from_rows(C, Vec<T>{10, 20, 30});
    CHECK(allclose(C, A));
    CHECK_THROWS(add_to_rows(C, Vec<T>{1, 2}), SizeMismatchError);

    Vec<T> means = centre_columns(C);
    CHECK(allclose(means, Vec<T>{2.5, 3.5, 4.5}));
    CHECK(allclose(C.mean_cols(), Vec<T>(3), T(0), T(1e-6)));
}

void test_printing() {
    Matrixd A{{1, 2.5}, {-3, 4}};
    CHECK(to_string(A) == "[[ 1, 2.5],\n [-3,   4]]");
    std::ostringstream os;
    os << Matrixd{{0.1 + 0.2}};
    CHECK(os.str() == "[[0.3]]");
    CHECK(to_string(Matrixd()) == "[]");
}

void test_errors_messages() {
    try {
        Matrixd(2, 3).at(5, 0);
    } catch (const IndexError& e) {
        CHECK(std::string(e.what()).find("row index 5") != std::string::npos);
        CHECK(e.index() == 5 && e.size() == 2);
    }
    try {
        (void)(Matrixd(2, 3) + Matrixd(3, 2));
    } catch (const ShapeMismatchError& e) {
        CHECK(std::string(e.what()).find("2 x 3 and 3 x 2") != std::string::npos);
        CHECK(e.left_rows() == 2 && e.right_cols() == 2);
    }
    const std::size_t huge = std::size_t(1) << 40;
    CHECK_THROWS(Matrixd(huge, huge), AllocationSizeError);
}

}

int main() {
    test_vec_new_features<double>();
    test_vec_new_features<float>();
    test_matrix_construction<double>();
    test_matrix_construction<float>();
    test_matrix_arithmetic<double>();
    test_matrix_arithmetic<float>();
    test_matrix_transpose_and_diagonals<double>();
    test_matrix_transpose_and_diagonals<float>();
    test_matrix_rows_and_reductions<double>();
    test_matrix_rows_and_reductions<float>();
    test_printing();
    test_errors_messages();

    std::printf("%d checks, %d failed\n", g_checks, g_failures);
    return g_failures == 0 ? 0 : 1;
}
