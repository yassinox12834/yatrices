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

#include "internal.hpp"

namespace yatrices {
namespace {

using detail::aligned;
using detail::allocate;
using detail::map_kernel;
using detail::release;
using detail::sum_map_kernel;
using detail::sum_zip_kernel;
using detail::update_kernel;
using detail::zip_kernel;

[[noreturn]] void throw_size_mismatch(const char* who, std::size_t a, std::size_t b) {
    throw SizeMismatchError(who, a, b);
}

}

static_assert(Vec<double>::alignment == detail::kAlignment,
              "Vec::alignment and the allocator must agree");

template <typename T>
Vec<T>::Vec(std::size_t n) : data_(allocate<T>(n)), size_(n) {
    if (n != 0) std::memset(data_, 0, n * sizeof(T));
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
        Vec copy(other);
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
    if (this == &x) {
        for (std::size_t i = 0; i < size_; ++i) data_[i] += alpha * data_[i];
        return;
    }
    detail::axpy_kernel(aligned(data_), aligned(x.data_), alpha, size_);
}

template <typename T>
T Vec<T>::dot(const Vec& other) const {
    if (size_ != other.size_) throw_size_mismatch("Vec::dot", size_, other.size_);
    if (size_ == 0) return T{0};
    return detail::dot_kernel(aligned(data_), aligned(other.data_), size_);
}

template <typename T>
Vec<T>& Vec<T>::operator+=(const Vec& other) {
    if (size_ != other.size_) throw_size_mismatch("Vec::operator+=", size_, other.size_);
    if (size_ == 0) return *this;
    if (this == &other) {
        for (std::size_t i = 0; i < size_; ++i) data_[i] += data_[i];
        return *this;
    }
    update_kernel(aligned(data_), aligned(other.data_), size_, [](T p, T q) { return p + q; });
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
    update_kernel(aligned(data_), aligned(other.data_), size_, [](T p, T q) { return p - q; });
    return *this;
}

template <typename T>
Vec<T>& Vec<T>::operator*=(T alpha) noexcept {
    scale(alpha);
    return *this;
}

template <typename T>
Vec<T>& Vec<T>::operator/=(T alpha) noexcept {
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
        if (std::isnan(a)) return a;
        if (a > best) best = a;
    }
    return best;
}

template <typename T>
T Vec<T>::norm2() const noexcept {
    if (size_ == 0) return T{0};
    const T sumsq = sum_map_kernel(aligned(data_), size_, [](T v) { return v * v; });
    constexpr T lowest_safe = std::numeric_limits<T>::min() / std::numeric_limits<T>::epsilon();
    if (std::isfinite(sumsq) && sumsq >= lowest_safe) return std::sqrt(sumsq);

    const T m = norm_inf();
    if (m == T{0} || !std::isfinite(m)) return m;
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
    Vec<T> out(x.size(), uninitialised);
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
    const T c = std::clamp(x.dot(y) / (nx * ny), T{-1}, T{1});
    return std::acos(c);
}

template <typename T>
Vec<T> project(const Vec<T>& v, const Vec<T>& onto) {
    if (v.size() != onto.size()) throw_size_mismatch("project", v.size(), onto.size());
    const T uu = onto.dot(onto);
    if (uu == T{0}) throw ZeroVectorError("project");
    const T factor = onto.dot(v) / uu;
    Vec<T> out(onto.size(), uninitialised);
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

template <typename T>
Vec<T> Vec<T>::ones(std::size_t n) {
    return Vec(n, T{1});
}

template <typename T>
Vec<T> Vec<T>::unit(std::size_t n, std::size_t k) {
    if (k >= n) throw IndexError("Vec::unit", k, n);
    Vec e(n);
    e.data_[k] = T{1};
    return e;
}

template <typename T>
Vec<T> Vec<T>::linspace(T a, T b, std::size_t n) {
    Vec out(n, uninitialised);
    if (n == 0) return out;
    if (n == 1) {
        out.data_[0] = a;
        return out;
    }
    const T step = (b - a) / static_cast<T>(n - 1);
    for (std::size_t i = 0; i + 1 < n; ++i) out.data_[i] = a + static_cast<T>(i) * step;
    out.data_[n - 1] = b;
    return out;
}

template <typename T>
std::size_t Vec<T>::argmin() const {
    if (size_ == 0) throw EmptyVectorError("Vec::argmin");
    std::size_t best = 0;
    for (std::size_t i = 0; i < size_; ++i) {
        if (std::isnan(data_[i])) return i;
        if (data_[i] < data_[best]) best = i;
    }
    return best;
}

template <typename T>
std::size_t Vec<T>::argmax() const {
    if (size_ == 0) throw EmptyVectorError("Vec::argmax");
    std::size_t best = 0;
    for (std::size_t i = 0; i < size_; ++i) {
        if (std::isnan(data_[i])) return i;
        if (data_[i] > data_[best]) best = i;
    }
    return best;
}

template <typename T>
T Vec<T>::prod() const noexcept {
    T p{1};
    for (std::size_t i = 0; i < size_; ++i) p *= data_[i];
    return p;
}

template <typename T>
T Vec<T>::variance(bool sample) const {
    if (size_ == 0) throw EmptyVectorError("Vec::variance");
    if (sample && size_ < 2) throw WrongSizeError("Vec::variance (sample)", size_, 2);
    const T m = mean();
    const T ss = sum_map_kernel(aligned(data_), size_, [m](T v) { return (v - m) * (v - m); });
    return ss / static_cast<T>(sample ? size_ - 1 : size_);
}

template <typename T>
T Vec<T>::stddev(bool sample) const {
    return std::sqrt(variance(sample));
}

template <typename T>
void Vec<T>::normalise() {
    const T n = norm2();
    if (n == T{0}) throw ZeroVectorError("Vec::normalise");
    *this /= n;
}

template <typename T>
Vec<T> Vec<T>::normalised() const {
    const T n = norm2();
    if (n == T{0}) throw ZeroVectorError("Vec::normalised");
    return *this / n;
}

template <typename T>
std::size_t Vec<T>::count_nonzero(T tol) const noexcept {
    std::size_t c = 0;
    for (std::size_t i = 0; i < size_; ++i) c += (std::fabs(data_[i]) > tol) ? 1 : 0;
    return c;
}

template <typename T>
bool Vec<T>::has_nan() const noexcept {
    for (std::size_t i = 0; i < size_; ++i) {
        if (std::isnan(data_[i])) return true;
    }
    return false;
}

template <typename T>
Vec<T> lincomb(non_deduced_t<T> alpha, const Vec<T>& u, non_deduced_t<T> beta,
               const Vec<T>& v) {
    if (u.size() != v.size()) throw_size_mismatch("lincomb", u.size(), v.size());
    Vec<T> out(u.size(), uninitialised);
    if (!u.empty()) {
        zip_kernel(aligned(out.data()), aligned(u.data()), aligned(v.data()), u.size(),
                   [alpha, beta](T p, T q) { return alpha * p + beta * q; });
    }
    return out;
}

template <typename T>
Vec<T> lerp(const Vec<T>& u, const Vec<T>& v, non_deduced_t<T> t) {
    if (u.size() != v.size()) throw_size_mismatch("lerp", u.size(), v.size());
    return lincomb(T{1} - t, u, t, v);
}

template <typename T>
T distance(const Vec<T>& x, const Vec<T>& y) {
    if (x.size() != y.size()) throw_size_mismatch("distance", x.size(), y.size());
    if (x.empty()) return T{0};
    const T ss = sum_zip_kernel(aligned(x.data()), aligned(y.data()), x.size(),
                                [](T p, T q) { return (p - q) * (p - q); });
    constexpr T lowest_safe =
        std::numeric_limits<T>::min() / std::numeric_limits<T>::epsilon();
    if (std::isfinite(ss) && ss >= lowest_safe) return std::sqrt(ss);
    if (ss == T{0} && distance_inf(x, y) == T{0}) return T{0};
    return (x - y).norm2();
}

template <typename T>
T distance1(const Vec<T>& x, const Vec<T>& y) {
    if (x.size() != y.size()) throw_size_mismatch("distance1", x.size(), y.size());
    if (x.empty()) return T{0};
    return sum_zip_kernel(aligned(x.data()), aligned(y.data()), x.size(),
                          [](T p, T q) { return std::fabs(p - q); });
}

template <typename T>
T distance_inf(const Vec<T>& x, const Vec<T>& y) {
    if (x.size() != y.size()) throw_size_mismatch("distance_inf", x.size(), y.size());
    T best{0};
    for (std::size_t i = 0; i < x.size(); ++i) {
        const T d = std::fabs(x[i] - y[i]);
        if (std::isnan(d)) return d;
        if (d > best) best = d;
    }
    return best;
}

template <typename T>
T cosine_similarity(const Vec<T>& u, const Vec<T>& v) {
    if (u.size() != v.size()) throw_size_mismatch("cosine_similarity", u.size(), v.size());
    const T nu = u.norm2();
    const T nv = v.norm2();
    if (nu == T{0} || nv == T{0}) throw ZeroVectorError("cosine_similarity");
    return std::clamp(u.dot(v) / (nu * nv), T{-1}, T{1});
}

template <typename T>
bool operator==(const Vec<T>& x, const Vec<T>& y) noexcept {
    if (x.size() != y.size()) return false;
    for (std::size_t i = 0; i < x.size(); ++i) {
        if (x[i] != y[i]) return false;
    }
    return true;
}

template <typename T>
bool operator!=(const Vec<T>& x, const Vec<T>& y) noexcept {
    return !(x == y);
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

template Vec<float> lincomb<float>(float, const Vec<float>&, float, const Vec<float>&);
template Vec<float> lerp<float>(const Vec<float>&, const Vec<float>&, float);
template float distance<float>(const Vec<float>&, const Vec<float>&);
template float distance1<float>(const Vec<float>&, const Vec<float>&);
template float distance_inf<float>(const Vec<float>&, const Vec<float>&);
template float cosine_similarity<float>(const Vec<float>&, const Vec<float>&);
template bool operator==<float>(const Vec<float>&, const Vec<float>&) noexcept;
template bool operator!=<float>(const Vec<float>&, const Vec<float>&) noexcept;
template Vec<double> lincomb<double>(double, const Vec<double>&, double, const Vec<double>&);
template Vec<double> lerp<double>(const Vec<double>&, const Vec<double>&, double);
template double distance<double>(const Vec<double>&, const Vec<double>&);
template double distance1<double>(const Vec<double>&, const Vec<double>&);
template double distance_inf<double>(const Vec<double>&, const Vec<double>&);
template double cosine_similarity<double>(const Vec<double>&, const Vec<double>&);
template bool operator==<double>(const Vec<double>&, const Vec<double>&) noexcept;
template bool operator!=<double>(const Vec<double>&, const Vec<double>&) noexcept;

}
