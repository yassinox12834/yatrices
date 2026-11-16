#include "yatrices/vector.hpp"

#include "yatrices/errors.hpp"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <new>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>

#if defined(__GNUC__) || defined(__clang__)
#define YATRICES_RESTRICT __restrict__
#elif defined(_MSC_VER)
#define YATRICES_RESTRICT __restrict
#else
#define YATRICES_RESTRICT
#endif

namespace yatrices {
namespace {

constexpr std::align_val_t kAlign{Vec<double>::alignment};

// Tell the compiler the pointer is 64-byte aligned (it always is, see allocate).
template <typename T>
inline T* aligned(T* p) noexcept {
#if defined(__GNUC__) || defined(__clang__)
    return static_cast<T*>(__builtin_assume_aligned(p, Vec<double>::alignment));
#else
    return p;
#endif
}

template <typename T>
T* allocate(std::size_t n) {
    if (n == 0) return nullptr;
    if (n > static_cast<std::size_t>(-1) / sizeof(T)) {
        throw AllocationSizeError(n, sizeof(T));
    }
    try {
        return static_cast<T*>(::operator new(n * sizeof(T), kAlign));
    } catch (const std::bad_alloc&) {
        throw OutOfMemoryError(n, sizeof(T));
    }
}

void release(void* p) noexcept {
    if (p != nullptr) ::operator delete(p, kAlign);
}

// Number of independent running sums in dot(). A float add has a latency of
// about 4 cycles but the CPU can start 2 per cycle, so one running sum leaves
// the adder idle most of the time. 128 bytes of sums = 8 AVX2 registers.
template <typename T>
constexpr std::size_t kSums = 128 / sizeof(T);   // 32 floats, 16 doubles

template <typename T>
T dot_kernel(const T* YATRICES_RESTRICT a, const T* YATRICES_RESTRICT b,
             std::size_t n) noexcept {
    constexpr std::size_t K = kSums<T>;
    T sums[K] = {};
    std::size_t i = 0;
    for (; i + K <= n; i += K) {
        for (std::size_t j = 0; j < K; ++j) sums[j] += a[i + j] * b[i + j];
    }
    for (std::size_t w = K / 2; w > 0; w /= 2) {        // pairwise reduction
        for (std::size_t j = 0; j < w; ++j) sums[j] += sums[j + w];
    }
    T total = sums[0];
    for (; i < n; ++i) total += a[i] * b[i];
    return total;
}

template <typename T>
void axpy_kernel(T* YATRICES_RESTRICT y, const T* YATRICES_RESTRICT x, T alpha,
                 std::size_t n) noexcept {
    for (std::size_t i = 0; i < n; ++i) y[i] += alpha * x[i];
}

// Element-wise kernels. __restrict__ promises the pointers do not overlap, which
// lets the compiler load and store whole SIMD registers at a time.
template <typename T, typename Op>
void zip_kernel(T* YATRICES_RESTRICT out, const T* YATRICES_RESTRICT a,
                const T* YATRICES_RESTRICT b, std::size_t n, Op op) noexcept {
    for (std::size_t i = 0; i < n; ++i) out[i] = op(a[i], b[i]);
}

template <typename T, typename Op>
void map_kernel(T* YATRICES_RESTRICT out, const T* YATRICES_RESTRICT a, std::size_t n,
                Op op) noexcept {
    for (std::size_t i = 0; i < n; ++i) out[i] = op(a[i]);
}

// Sum of f(a_i) with kSums independent running sums (the idea of dot_kernel).
template <typename T, typename F>
T sum_map_kernel(const T* a, std::size_t n, F f) noexcept {
    constexpr std::size_t K = kSums<T>;
    T sums[K] = {};
    std::size_t i = 0;
    for (; i + K <= n; i += K) {
        for (std::size_t j = 0; j < K; ++j) sums[j] += f(a[i + j]);
    }
    for (std::size_t w = K / 2; w > 0; w /= 2) {
        for (std::size_t j = 0; j < w; ++j) sums[j] += sums[j + w];
    }
    T total = sums[0];
    for (; i < n; ++i) total += f(a[i]);
    return total;
}

[[noreturn]] void throw_size_mismatch(const char* who, std::size_t a, std::size_t b) {
    throw SizeMismatchError(who, a, b);
}

}  // namespace

template <typename T>
Vec<T>::Vec(std::size_t n) : data_(allocate<T>(n)), size_(n) {
    if (n != 0) std::memset(data_, 0, n * sizeof(T));   // all-bits-zero is 0.0
}

template <typename T>
Vec<T>::Vec(std::size_t n, T value) : data_(allocate<T>(n)), size_(n) {
    std::fill(data_, data_ + n, value);
}

template <typename T>
Vec<T>::Vec(std::size_t n, UninitialisedTag) : data_(allocate<T>(n)), size_(n) {}

template <typename T>
Vec<T>::Vec(std::initializer_list<T> xs)
    : data_(allocate<T>(xs.size())), size_(xs.size()) {
    std::copy(xs.begin(), xs.end(), data_);
}

template <typename T>
Vec<T>::Vec(const Vec& other) : data_(allocate<T>(other.size_)), size_(other.size_) {
    if (size_ != 0) std::memcpy(data_, other.data_, size_ * sizeof(T));
}

template <typename T>
Vec<T>::Vec(Vec&& other) noexcept
    : data_(std::exchange(other.data_, nullptr)),
      size_(std::exchange(other.size_, 0)) {}

template <typename T>
Vec<T>& Vec<T>::operator=(const Vec& other) {
    if (this == &other) return *this;
    if (size_ == other.size_) {
        if (size_ != 0) std::memcpy(data_, other.data_, size_ * sizeof(T));
    } else {
        Vec copy(other);                        // may throw; *this untouched
        swap(copy);
    }
    return *this;
}

template <typename T>
Vec<T>& Vec<T>::operator=(Vec&& other) noexcept {
    if (this != &other) {
        release(data_);
        data_ = std::exchange(other.data_, nullptr);
        size_ = std::exchange(other.size_, 0);
    }
    return *this;
}

template <typename T>
Vec<T>::~Vec() {
    release(data_);
}

template <typename T>
void Vec<T>::swap(Vec& other) noexcept {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
}

template <typename T>
T& Vec<T>::at(std::size_t i) {
    if (i >= size_) throw IndexError("Vec::at", i, size_);
    return data_[i];
}

template <typename T>
const T& Vec<T>::at(std::size_t i) const {
    if (i >= size_) throw IndexError("Vec::at", i, size_);
    return data_[i];
}

template <typename T>
void Vec<T>::fill(T value) noexcept {
    if (size_ != 0) std::fill(aligned(data_), aligned(data_) + size_, value);
}

template <typename T>
void Vec<T>::scale(T alpha) noexcept {
    T* YATRICES_RESTRICT p = aligned(data_);
    for (std::size_t i = 0; i < size_; ++i) p[i] *= alpha;
}

template <typename T>
void Vec<T>::axpy(T alpha, const Vec& x) {
    if (size_ != x.size_) throw_size_mismatch("Vec::axpy", size_, x.size_);
    if (size_ == 0) return;
    if (this == &x) {                           // aliasing: restrict would be a lie
        for (std::size_t i = 0; i < size_; ++i) data_[i] += alpha * data_[i];
        return;
    }
    axpy_kernel(aligned(data_), aligned(x.data_), alpha, size_);
}

template <typename T>
T Vec<T>::dot(const Vec& other) const {
    if (size_ != other.size_) throw_size_mismatch("Vec::dot", size_, other.size_);
    if (size_ == 0) return T{0};
    return dot_kernel(aligned(data_), aligned(other.data_), size_);
}

template <typename T>
Vec<T>& Vec<T>::operator+=(const Vec& other) {
    if (size_ != other.size_) throw_size_mismatch("Vec::operator+=", size_, other.size_);
    if (size_ == 0) return *this;
    if (this == &other) {                       // x += x: restrict would be a lie
        for (std::size_t i = 0; i < size_; ++i) data_[i] += data_[i];
        return *this;
    }
    // y_i = y_i + x_i. No multiplication by 1, and the size is checked only once.
    zip_kernel(aligned(data_), aligned(data_), aligned(other.data_), size_,
               [](T p, T q) { return p + q; });
    return *this;
}

template <typename T>
Vec<T>& Vec<T>::operator-=(const Vec& other) {
    if (size_ != other.size_) throw_size_mismatch("Vec::operator-=", size_, other.size_);
    if (size_ == 0) return *this;
    if (this == &other) {
        for (std::size_t i = 0; i < size_; ++i) data_[i] -= data_[i];
        return *this;
    }
    zip_kernel(aligned(data_), aligned(data_), aligned(other.data_), size_,
               [](T p, T q) { return p - q; });
    return *this;
}

template <typename T>
Vec<T>& Vec<T>::operator*=(T alpha) noexcept {
    scale(alpha);
    return *this;
}

template <typename T>
Vec<T>& Vec<T>::operator/=(T alpha) noexcept {
    // Real division, not scale(1 / alpha): 1 / alpha is rounded once more, and
    // for a tiny alpha (below about 1e-308) it overflows to infinity.
    T* YATRICES_RESTRICT p = aligned(data_);
    for (std::size_t i = 0; i < size_; ++i) p[i] /= alpha;
    return *this;
}

template <typename T>
T Vec<T>::norm1() const noexcept {
    if (size_ == 0) return T{0};
    return sum_map_kernel(aligned(data_), size_, [](T v) { return std::fabs(v); });
}

template <typename T>
T Vec<T>::norm_inf() const noexcept {
    T best{0};
    for (std::size_t i = 0; i < size_; ++i) {
        const T a = std::fabs(data_[i]);
        if (std::isnan(a)) return a;            // a comparison with NaN is always false
        if (a > best) best = a;
    }
    return best;
}

template <typename T>
T Vec<T>::norm2() const noexcept {
    if (size_ == 0) return T{0};
    // Fast path: ONE pass over the data. Overflow cannot hide in it, because the
    // running sums only grow, so a finite final sum means no step was infinite.
    // Underflow (squares of tiny numbers rounding to 0) is caught by the lower bound.
    const T sumsq = sum_map_kernel(aligned(data_), size_, [](T v) { return v * v; });
    constexpr T lowest_safe = std::numeric_limits<T>::min() / std::numeric_limits<T>::epsilon();
    if (std::isfinite(sumsq) && sumsq >= lowest_safe) return std::sqrt(sumsq);

    // Slow path (huge, tiny, infinite or NaN values): two passes, rescaled.
    // ||x|| = m * sqrt(sum (x_i / m)^2) with m = max |x_i|, so every x_i / m is in [-1, 1].
    const T m = norm_inf();
    if (m == T{0} || !std::isfinite(m)) return m;   // zeros, infinity or NaN
    T total{0};
    for (std::size_t i = 0; i < size_; ++i) {
        const T r = data_[i] / m;
        total += r * r;
    }
    return m * std::sqrt(total);
}

template <typename T>
T Vec<T>::sum() const noexcept {
    if (size_ == 0) return T{0};
    return sum_map_kernel(aligned(data_), size_, [](T v) { return v; });
}

template <typename T>
T Vec<T>::mean() const {
    if (size_ == 0) throw EmptyVectorError("Vec::mean");
    return sum() / static_cast<T>(size_);
}

template <typename T>
T Vec<T>::min() const {
    if (size_ == 0) throw EmptyVectorError("Vec::min");
    return *std::min_element(data_, data_ + size_);
}

template <typename T>
T Vec<T>::max() const {
    if (size_ == 0) throw EmptyVectorError("Vec::max");
    return *std::max_element(data_, data_ + size_);
}

template <typename T>
Vec<T> operator+(const Vec<T>& a, const Vec<T>& b) {
    if (a.size() != b.size()) throw_size_mismatch("operator+", a.size(), b.size());
    Vec<T> out(a.size(), uninitialised);
    if (!a.empty()) {
        zip_kernel(aligned(out.data()), aligned(a.data()), aligned(b.data()), a.size(),
                   [](T p, T q) { return p + q; });
    }
    return out;
}

template <typename T>
Vec<T> operator-(const Vec<T>& a, const Vec<T>& b) {
    if (a.size() != b.size()) throw_size_mismatch("operator-", a.size(), b.size());
    Vec<T> out(a.size(), uninitialised);
    if (!a.empty()) {
        zip_kernel(aligned(out.data()), aligned(a.data()), aligned(b.data()), a.size(),
                   [](T p, T q) { return p - q; });
    }
    return out;
}

template <typename T>
Vec<T> operator-(const Vec<T>& a) {
    Vec<T> out(a.size(), uninitialised);
    if (!a.empty()) {
        map_kernel(aligned(out.data()), aligned(a.data()), a.size(), [](T p) { return -p; });
    }
    return out;
}

template <typename T>
Vec<T> operator*(const Vec<T>& a, non_deduced_t<T> alpha) {
    Vec<T> out(a.size(), uninitialised);
    if (!a.empty()) {
        map_kernel(aligned(out.data()), aligned(a.data()), a.size(),
                   [alpha](T p) { return p * alpha; });
    }
    return out;
}

template <typename T>
Vec<T> operator*(non_deduced_t<T> alpha, const Vec<T>& a) { return a * alpha; }

template <typename T>
Vec<T> operator/(const Vec<T>& a, non_deduced_t<T> alpha) {
    Vec<T> out(a.size(), uninitialised);
    if (!a.empty()) {
        map_kernel(aligned(out.data()), aligned(a.data()), a.size(),
                   [alpha](T p) { return p / alpha; });
    }
    return out;
}

template <typename T>
Vec<T> hadamard(const Vec<T>& x, const Vec<T>& y) {
    if (x.size() != y.size()) throw_size_mismatch("hadamard", x.size(), y.size());
    Vec<T> out(x.size(), uninitialised);        // every element is written below
    if (!x.empty()) {
        zip_kernel(aligned(out.data()), aligned(x.data()), aligned(y.data()), x.size(),
                   [](T p, T q) { return p * q; });
    }
    return out;
}

template <typename T>
Vec<T> cross(const Vec<T>& x, const Vec<T>& y) {
    if (x.size() != 3) throw WrongSizeError("cross", x.size(), 3);
    if (y.size() != 3) throw WrongSizeError("cross", y.size(), 3);
    return Vec<T>{x[1] * y[2] - x[2] * y[1],
                  x[2] * y[0] - x[0] * y[2],
                  x[0] * y[1] - x[1] * y[0]};
}

template <typename T>
T angle(const Vec<T>& x, const Vec<T>& y) {
    if (x.size() != y.size()) throw_size_mismatch("angle", x.size(), y.size());
    const T nx = x.norm2();
    const T ny = y.norm2();
    if (nx == T{0} || ny == T{0}) throw ZeroVectorError("angle");
    // Rounding can push the cosine just past 1 (1.0000000000000002), and
    // acos of that is NaN, so clamp it back into [-1, 1].
    const T c = std::clamp(x.dot(y) / (nx * ny), T{-1}, T{1});
    return std::acos(c);
}

template <typename T>
Vec<T> project(const Vec<T>& v, const Vec<T>& onto) {
    if (v.size() != onto.size()) throw_size_mismatch("project", v.size(), onto.size());
    const T uu = onto.dot(onto);
    if (uu == T{0}) throw ZeroVectorError("project");
    const T factor = onto.dot(v) / uu;
    Vec<T> out(onto.size(), uninitialised);     // one pass, no copy-then-scale
    map_kernel(aligned(out.data()), aligned(onto.data()), onto.size(),
               [factor](T p) { return p * factor; });
    return out;
}

template <typename T>
bool allclose(const Vec<T>& x, const Vec<T>& y, non_deduced_t<T> rtol,
              non_deduced_t<T> atol) noexcept {
    if (x.size() != y.size()) return false;
    for (std::size_t i = 0; i < x.size(); ++i) {
        if (!approx_equal(x[i], y[i], rtol, atol)) return false;
    }
    return true;
}

template <typename T>
bool Vec<T>::all_finite() const noexcept {
    for (std::size_t i = 0; i < size_; ++i) {
        if (!std::isfinite(data_[i])) return false;
    }
    return true;
}

template <typename T>
std::string to_string(const Vec<T>& v, int digits) {
    if (digits < 1) digits = 1;
    std::string out = "[";
    char buf[64];
    for (std::size_t i = 0; i < v.size(); ++i) {
        if (i != 0) out += ", ";
        std::snprintf(buf, sizeof buf, "%.*g", digits, static_cast<double>(v[i]));
        out += buf;
    }
    return out + "]";
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vec<T>& v) {
    return os << to_string(v);
}

template class Vec<float>;
template class Vec<double>;
template Vec<float> operator+<float>(const Vec<float>&, const Vec<float>&);
template Vec<float> operator-<float>(const Vec<float>&, const Vec<float>&);
template Vec<float> operator-<float>(const Vec<float>&);
template Vec<float> operator*<float>(const Vec<float>&, float);
template Vec<float> operator*<float>(float, const Vec<float>&);
template Vec<float> operator/<float>(const Vec<float>&, float);
template Vec<double> operator+<double>(const Vec<double>&, const Vec<double>&);
template Vec<double> operator-<double>(const Vec<double>&, const Vec<double>&);
template Vec<double> operator-<double>(const Vec<double>&);
template Vec<double> operator*<double>(const Vec<double>&, double);
template Vec<double> operator*<double>(double, const Vec<double>&);
template Vec<double> operator/<double>(const Vec<double>&, double);
template Vec<float>  hadamard<float>(const Vec<float>&, const Vec<float>&);
template Vec<double> hadamard<double>(const Vec<double>&, const Vec<double>&);
template Vec<float>  cross<float>(const Vec<float>&, const Vec<float>&);
template Vec<double> cross<double>(const Vec<double>&, const Vec<double>&);
template float  angle<float>(const Vec<float>&, const Vec<float>&);
template double angle<double>(const Vec<double>&, const Vec<double>&);
template Vec<float>  project<float>(const Vec<float>&, const Vec<float>&);
template Vec<double> project<double>(const Vec<double>&, const Vec<double>&);
template bool allclose<float>(const Vec<float>&, const Vec<float>&, float, float) noexcept;
template bool allclose<double>(const Vec<double>&, const Vec<double>&, double, double) noexcept;
template std::string to_string<float>(const Vec<float>&, int);
template std::string to_string<double>(const Vec<double>&, int);
template std::ostream& operator<< <float>(std::ostream&, const Vec<float>&);
template std::ostream& operator<< <double>(std::ostream&, const Vec<double>&);

}  // namespace yatrices
