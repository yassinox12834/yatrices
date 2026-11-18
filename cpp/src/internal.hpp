#pragma once

#include "yatrices/errors.hpp"

#include <cstddef>
#include <limits>
#include <new>

#if defined(__GNUC__) || defined(__clang__)
#define YATRICES_RESTRICT __restrict__
#elif defined(_MSC_VER)
#define YATRICES_RESTRICT __restrict
#else
#define YATRICES_RESTRICT
#endif

namespace yatrices {
namespace detail {

inline constexpr std::size_t kAlignment = 64;
inline constexpr std::align_val_t kAlign{kAlignment};

template <typename T>
inline T* aligned(T* p) noexcept {
#if defined(__GNUC__) || defined(__clang__)
    return static_cast<T*>(__builtin_assume_aligned(p, kAlignment));
#else
    return p;
#endif
}

template <typename T>
T* allocate(std::size_t n) {
    if (n == 0) return nullptr;
    if (n > std::numeric_limits<std::size_t>::max() / sizeof(T)) {
        throw AllocationSizeError(n, sizeof(T));
    }
    try {
        return static_cast<T*>(::operator new(n * sizeof(T), kAlign));
    } catch (const std::bad_alloc&) {
        throw OutOfMemoryError(n, sizeof(T));
    }
}

template <typename T>
std::size_t checked_count(std::size_t rows, std::size_t cols) {
    if (cols != 0 && rows > std::numeric_limits<std::size_t>::max() / cols) {
        throw AllocationSizeError(rows, cols, sizeof(T));
    }
    return rows * cols;
}

inline void release(void* p) noexcept {
    if (p != nullptr) ::operator delete(p, kAlign);
}

template <typename T>
inline constexpr std::size_t kSums = 128 / sizeof(T);

template <typename T>
T dot_kernel(const T* YATRICES_RESTRICT a, const T* YATRICES_RESTRICT b,
             std::size_t n) noexcept {
    constexpr std::size_t K = kSums<T>;
    T sums[K] = {};
    std::size_t i = 0;
    for (; i + K <= n; i += K) {
        for (std::size_t j = 0; j < K; ++j) sums[j] += a[i + j] * b[i + j];
    }
    for (std::size_t w = K / 2; w > 0; w /= 2) {
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

template <typename T, typename Op>
void zip_kernel(T* YATRICES_RESTRICT out, const T* YATRICES_RESTRICT a,
                const T* YATRICES_RESTRICT b, std::size_t n, Op op) noexcept {
    for (std::size_t i = 0; i < n; ++i) out[i] = op(a[i], b[i]);
}

template <typename T, typename Op>
void update_kernel(T* YATRICES_RESTRICT a, const T* YATRICES_RESTRICT b, std::size_t n,
                   Op op) noexcept {
    for (std::size_t i = 0; i < n; ++i) a[i] = op(a[i], b[i]);
}

template <typename T, typename Op>
void map_kernel(T* YATRICES_RESTRICT out, const T* YATRICES_RESTRICT a, std::size_t n,
                Op op) noexcept {
    for (std::size_t i = 0; i < n; ++i) out[i] = op(a[i]);
}

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

template <typename T, typename F>
T sum_zip_kernel(const T* YATRICES_RESTRICT a, const T* YATRICES_RESTRICT b, std::size_t n,
                 F f) noexcept {
    constexpr std::size_t K = kSums<T>;
    T sums[K] = {};
    std::size_t i = 0;
    for (; i + K <= n; i += K) {
        for (std::size_t j = 0; j < K; ++j) sums[j] += f(a[i + j], b[i + j]);
    }
    for (std::size_t w = K / 2; w > 0; w /= 2) {
        for (std::size_t j = 0; j < w; ++j) sums[j] += sums[j + w];
    }
    T total = sums[0];
    for (; i < n; ++i) total += f(a[i], b[i]);
    return total;
}

}
}
