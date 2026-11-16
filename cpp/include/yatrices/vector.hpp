#pragma once

#include <cstddef>
#include <algorithm>
#include <cmath>
#include <initializer_list>
#include <iosfwd>
#include <limits>
#include <string>
#include <type_traits>
#include <utility>

namespace yatrices {

// Tag meaning "give me the memory, do not zero it". Use it when you are about
// to overwrite every element anyway, so the zeroing pass is wasted work.
struct UninitialisedTag {
    explicit UninitialisedTag() = default;
};
inline constexpr UninitialisedTag uninitialised{};

// "Do not use this argument to work out T." A free function like operator*(T, Vec<T>)
// must find T from its arguments. With 2 * x, the 2 says T = int and the x says
// T = double, so the compiler gives up. Writing the scalar as non_deduced_t<T>
// makes ONLY the vector decide T; the scalar is then converted to it (int to
// double for a literal happens at compile time, so it costs nothing at run time).
template <typename T>
struct non_deduced { using type = T; };
template <typename T>
using non_deduced_t = typename non_deduced<T>::type;

// Fixed-size dense vector of float or double.
//
// Memory design:
//   * 16 bytes per object (pointer + size). No capacity field, no slack:
//     the buffer holds exactly size() elements.
//   * The buffer starts on a 64-byte boundary (one cache line, one AVX-512
//     register), so the compiler can use aligned SIMD loads and no element
//     straddles two cache lines.
//   * Copies reuse the existing buffer when sizes match; moves never copy.
template <typename T>
class Vec {
    static_assert(std::is_same_v<T, float> || std::is_same_v<T, double>,
                  "Vec supports only float and double");

public:
    using value_type = T;
    static constexpr std::size_t alignment = 64;

    Vec() noexcept = default;
    explicit Vec(std::size_t n);                       // n zeros
    Vec(std::size_t n, T value);                       // n copies of value
    Vec(std::size_t n, UninitialisedTag);              // n elements, NOT zeroed
    Vec(std::initializer_list<T> xs);                  // {1.0, 2.0, 3.0}

    Vec(const Vec& other);
    Vec(Vec&& other) noexcept;
    Vec& operator=(const Vec& other);
    Vec& operator=(Vec&& other) noexcept;
    ~Vec();

    void swap(Vec& other) noexcept;

    [[nodiscard]] std::size_t size() const noexcept { return size_; }
    [[nodiscard]] bool empty() const noexcept { return size_ == 0; }

    T&       operator[](std::size_t i) noexcept       { return data_[i]; }
    const T& operator[](std::size_t i) const noexcept { return data_[i]; }
    T&       at(std::size_t i);                        // throws yatrices::IndexError
    const T& at(std::size_t i) const;

    T*       data() noexcept       { return data_; }
    const T* data() const noexcept { return data_; }
    T*       begin() noexcept       { return data_; }
    T*       end() noexcept         { return data_ + size_; }
    const T* begin() const noexcept { return data_; }
    const T* end() const noexcept   { return data_ + size_; }

    // fast kernels (throw yatrices::SizeMismatchError on size mismatch)
    void fill(T value) noexcept;
    void scale(T alpha) noexcept;
    void axpy(T alpha, const Vec& x);                  // this_i += alpha * x_i
    [[nodiscard]] T dot(const Vec& other) const;

    // in-place arithmetic (throw yatrices::SizeMismatchError on size mismatch)
    Vec& operator+=(const Vec& other);
    Vec& operator-=(const Vec& other);
    Vec& operator*=(T alpha) noexcept;
    Vec& operator/=(T alpha) noexcept;

    // norms (a NaN anywhere gives NaN)
    [[nodiscard]] T norm1() const noexcept;            // sum_i |x_i|
    [[nodiscard]] T norm2() const noexcept;            // sqrt(sum_i x_i^2), overflow-safe
    [[nodiscard]] T norm_inf() const noexcept;         // max_i |x_i|

    // reductions (an empty vector: sum is 0, the others throw EmptyVectorError)
    [[nodiscard]] T sum() const noexcept;
    [[nodiscard]] T mean() const;
    [[nodiscard]] T min() const;
    [[nodiscard]] T max() const;

    // false if any element is NaN or infinite. One bad value spreads silently
    // through every later operation, so check results you are unsure of.
    [[nodiscard]] bool all_finite() const noexcept;

private:
    T* data_ = nullptr;
    std::size_t size_ = 0;
};

// Every operator comes in two versions, and C++ picks one by looking at the
// left operand:
//   * a named vector (const Vec&): build the result in ONE pass into fresh,
//     unzeroed memory. "Copy a, then add b" would touch the memory twice.
//   * a temporary (Vec&&), like the (x + y) in x + y + z: that buffer is about
//     to be thrown away, so reuse it. No allocation, no copy.
// The scalar converts to the vector's type, so 2 * v and 0.5 * v both work.

template <typename T> [[nodiscard]] Vec<T> operator+(const Vec<T>& a, const Vec<T>& b);
template <typename T> [[nodiscard]] Vec<T> operator-(const Vec<T>& a, const Vec<T>& b);
template <typename T> [[nodiscard]] Vec<T> operator-(const Vec<T>& a);
template <typename T> [[nodiscard]] Vec<T> operator*(const Vec<T>& a, non_deduced_t<T> alpha);
template <typename T> [[nodiscard]] Vec<T> operator*(non_deduced_t<T> alpha, const Vec<T>& a);
template <typename T> [[nodiscard]] Vec<T> operator/(const Vec<T>& a, non_deduced_t<T> alpha);

template <typename T> [[nodiscard]] inline Vec<T> operator+(Vec<T>&& a, const Vec<T>& b) { a += b;        return std::move(a); }
template <typename T> [[nodiscard]] inline Vec<T> operator-(Vec<T>&& a, const Vec<T>& b) { a -= b;        return std::move(a); }
template <typename T> [[nodiscard]] inline Vec<T> operator-(Vec<T>&& a)                  { a.scale(T{-1}); return std::move(a); }
template <typename T> [[nodiscard]] inline Vec<T> operator*(Vec<T>&& a, non_deduced_t<T> alpha)         { a *= alpha;     return std::move(a); }
template <typename T> [[nodiscard]] inline Vec<T> operator*(non_deduced_t<T> alpha, Vec<T>&& a)         { a *= alpha;     return std::move(a); }
template <typename T> [[nodiscard]] inline Vec<T> operator/(Vec<T>&& a, non_deduced_t<T> alpha)         { a /= alpha;     return std::move(a); }

// Element-wise product: (x o y)_i = x_i * y_i. Throws SizeMismatchError.
template <typename T>
[[nodiscard]] Vec<T> hadamard(const Vec<T>& x, const Vec<T>& y);

// Cross product, defined only in 3 dimensions. Throws WrongSizeError.
template <typename T>
[[nodiscard]] Vec<T> cross(const Vec<T>& x, const Vec<T>& y);

// Angle between x and y in radians, in [0, pi]. Throws ZeroVectorError.
template <typename T>
[[nodiscard]] T angle(const Vec<T>& x, const Vec<T>& y);

// The part of v that points along `onto`: (onto . v) / (onto . onto) * onto.
// Throws ZeroVectorError if `onto` is all zeros.
template <typename T>
[[nodiscard]] Vec<T> project(const Vec<T>& v, const Vec<T>& onto);

// Never compare floats with ==. Typed values are rounded before any code runs:
//   0.1 + 0.2 is stored as 0.30000000000000004
//   0.3       is stored as 0.29999999999999999
// They are neighbours on the double grid (one ulp, about 5.55e-17, apart),
// but == compares exact bits, so it says false. Use approx_equal / allclose.

// Default tolerance: 1000 machine epsilons, about 1.2e-4 for float and
// 2.2e-13 for double. Room for about 1000 roundings to pile up.
template <typename T>
constexpr T default_tolerance() noexcept {
    return T(1000) * std::numeric_limits<T>::epsilon();
}

// True when |a - b| <= max(atol, rtol * max(|a|, |b|)).
//   rtol: relative tolerance, the error allowed per unit of size (large values)
//   atol: absolute tolerance, the fixed error allowed near zero
// NaN equals nothing; an infinity equals only the same infinity.
// a and b may have different types (approx_equal(x, 0) is fine): both are
// converted to their common type, and at least one must be float or double.
template <typename A, typename B>
[[nodiscard]] inline bool approx_equal(
        A a_in, B b_in,
        non_deduced_t<std::common_type_t<A, B>> rtol = default_tolerance<std::common_type_t<A, B>>(),
        non_deduced_t<std::common_type_t<A, B>> atol = default_tolerance<std::common_type_t<A, B>>()) noexcept {
    using T = std::common_type_t<A, B>;
    static_assert(std::is_floating_point_v<T>,
                  "approx_equal needs at least one float or double argument");
    const T a = static_cast<T>(a_in);
    const T b = static_cast<T>(b_in);
    if (a == b) return true;                                  // exact, or the same infinity
    if (!std::isfinite(a) || !std::isfinite(b)) return false; // NaN, or inf vs anything else
    const T diff  = std::fabs(a - b);
    const T scale = std::max(std::fabs(a), std::fabs(b));
    return diff <= std::max(atol, rtol * scale);
}

// Element-wise approx_equal. False if the sizes differ.
template <typename T>
[[nodiscard]] bool allclose(const Vec<T>& x, const Vec<T>& y,
                            non_deduced_t<T> rtol = default_tolerance<T>(),
                            non_deduced_t<T> atol = default_tolerance<T>()) noexcept;

// Default: std::numeric_limits<T>::digits10 significant digits (6 for float,
// 15 for double), the digits a value is guaranteed to keep. So 0.1 + 0.2
// prints as 0.3 while the stored value stays 0.30000000000000004.
// Pass max_digits10 (9 or 17) to see every stored digit when debugging.
template <typename T>
[[nodiscard]] std::string to_string(const Vec<T>& v,
                                    int digits = std::numeric_limits<T>::digits10);

// std::cout << v  prints  [0.3, 1, 2.5]  using the default digits.
template <typename T>
std::ostream& operator<<(std::ostream& os, const Vec<T>& v);

// Compiled once in vector.cpp, so every file that includes this header does
// not recompile the member functions.
extern template class Vec<float>;
extern template class Vec<double>;

using Vecf = Vec<float>;
using Vecd = Vec<double>;

}  // namespace yatrices
