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

struct UninitialisedTag {
    explicit UninitialisedTag() = default;
};
inline constexpr UninitialisedTag uninitialised{};

template <typename T>
struct non_deduced { using type = T; };
template <typename T>
using non_deduced_t = typename non_deduced<T>::type;

template <typename T>
class Vec {
    static_assert(std::is_same_v<T, float> || std::is_same_v<T, double>,
                  "Vec supports only float and double");

public:
    using value_type = T;
    static constexpr std::size_t alignment = 64;

    Vec() noexcept = default;
    explicit Vec(std::size_t n);
    Vec(std::size_t n, T value);
    Vec(std::size_t n, UninitialisedTag);
    Vec(std::initializer_list<T> xs);

    Vec(const Vec& other);
    Vec(Vec&& other) noexcept;
    Vec& operator=(const Vec& other);
    Vec& operator=(Vec&& other) noexcept;
    ~Vec();

    void swap(Vec& other) noexcept;

    [[nodiscard]] static Vec ones(std::size_t n);
    [[nodiscard]] static Vec unit(std::size_t n, std::size_t k);
    [[nodiscard]] static Vec linspace(T a, T b, std::size_t n);

    [[nodiscard]] std::size_t size() const noexcept { return size_; }
    [[nodiscard]] bool empty() const noexcept { return size_ == 0; }

    T&       operator[](std::size_t i) noexcept       { return data_[i]; }
    const T& operator[](std::size_t i) const noexcept { return data_[i]; }
    T&       at(std::size_t i);
    const T& at(std::size_t i) const;

    T*       data() noexcept       { return data_; }
    const T* data() const noexcept { return data_; }
    T*       begin() noexcept       { return data_; }
    T*       end() noexcept         { return data_ + size_; }
    const T* begin() const noexcept { return data_; }
    const T* end() const noexcept   { return data_ + size_; }

    void fill(T value) noexcept;
    void scale(T alpha) noexcept;
    void axpy(T alpha, const Vec& x);
    [[nodiscard]] T dot(const Vec& other) const;

    Vec& operator+=(const Vec& other);
    Vec& operator-=(const Vec& other);
    Vec& operator*=(T alpha) noexcept;
    Vec& operator/=(T alpha) noexcept;

    template <typename F>
    Vec& apply(F f) {
        for (std::size_t i = 0; i < size_; ++i) data_[i] = static_cast<T>(f(data_[i]));
        return *this;
    }

    [[nodiscard]] T norm1() const noexcept;
    [[nodiscard]] T norm2() const noexcept;
    [[nodiscard]] T norm_inf() const noexcept;

    [[nodiscard]] T sum() const noexcept;
    [[nodiscard]] T mean() const;
    [[nodiscard]] T min() const;
    [[nodiscard]] T max() const;
    [[nodiscard]] std::size_t argmin() const;
    [[nodiscard]] std::size_t argmax() const;
    [[nodiscard]] T prod() const noexcept;
    [[nodiscard]] T variance(bool sample = true) const;
    [[nodiscard]] T stddev(bool sample = true) const;

    void normalise();
    [[nodiscard]] Vec normalised() const;

    [[nodiscard]] std::size_t count_nonzero(T tol = T{0}) const noexcept;
    [[nodiscard]] bool has_nan() const noexcept;

    [[nodiscard]] bool all_finite() const noexcept;

private:
    T* data_ = nullptr;
    std::size_t size_ = 0;
};

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

template <typename T>
[[nodiscard]] Vec<T> hadamard(const Vec<T>& x, const Vec<T>& y);

template <typename T>
[[nodiscard]] Vec<T> cross(const Vec<T>& x, const Vec<T>& y);

template <typename T>
[[nodiscard]] T angle(const Vec<T>& x, const Vec<T>& y);

template <typename T>
[[nodiscard]] Vec<T> project(const Vec<T>& v, const Vec<T>& onto);

template <typename T>
[[nodiscard]] Vec<T> lincomb(non_deduced_t<T> alpha, const Vec<T>& u, non_deduced_t<T> beta,
                             const Vec<T>& v);

template <typename T>
[[nodiscard]] Vec<T> lerp(const Vec<T>& u, const Vec<T>& v, non_deduced_t<T> t);

template <typename T> [[nodiscard]] T distance(const Vec<T>& x, const Vec<T>& y);
template <typename T> [[nodiscard]] T distance1(const Vec<T>& x, const Vec<T>& y);
template <typename T> [[nodiscard]] T distance_inf(const Vec<T>& x, const Vec<T>& y);

template <typename T>
[[nodiscard]] T cosine_similarity(const Vec<T>& u, const Vec<T>& v);

template <typename T> [[nodiscard]] bool operator==(const Vec<T>& x, const Vec<T>& y) noexcept;
template <typename T> [[nodiscard]] bool operator!=(const Vec<T>& x, const Vec<T>& y) noexcept;

template <typename T>
constexpr T default_tolerance() noexcept {
    return T(1000) * std::numeric_limits<T>::epsilon();
}

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
    if (a == b) return true;
    if (!std::isfinite(a) || !std::isfinite(b)) return false;
    const T diff  = std::fabs(a - b);
    const T scale = std::max(std::fabs(a), std::fabs(b));
    return diff <= std::max(atol, rtol * scale);
}

template <typename T>
[[nodiscard]] bool allclose(const Vec<T>& x, const Vec<T>& y,
                            non_deduced_t<T> rtol = default_tolerance<T>(),
                            non_deduced_t<T> atol = default_tolerance<T>()) noexcept;

template <typename T>
[[nodiscard]] std::string to_string(const Vec<T>& v,
                                    int digits = std::numeric_limits<T>::digits10);

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vec<T>& v);

extern template class Vec<float>;
extern template class Vec<double>;

using Vecf = Vec<float>;
using Vecd = Vec<double>;

}
