#pragma once

#include <cstddef>
#include <initializer_list>
#include <iosfwd>
#include <limits>
#include <string>
#include <type_traits>
#include <utility>

#include "yatrices/vector.hpp"

namespace yatrices {

template <typename T>
class Matrix {
    static_assert(std::is_same_v<T, float> || std::is_same_v<T, double>,
                  "Matrix supports only float and double");

public:
    using value_type = T;
    static constexpr std::size_t alignment = Vec<T>::alignment;

    Matrix() noexcept = default;
    Matrix(std::size_t rows, std::size_t cols);
    Matrix(std::size_t rows, std::size_t cols, T value);
    Matrix(std::size_t rows, std::size_t cols, UninitialisedTag);
    Matrix(std::initializer_list<std::initializer_list<T>> rows);

    Matrix(const Matrix& other);
    Matrix(Matrix&& other) noexcept;
    Matrix& operator=(const Matrix& other);
    Matrix& operator=(Matrix&& other) noexcept;
    ~Matrix();

    void swap(Matrix& other) noexcept;

    [[nodiscard]] std::size_t rows() const noexcept { return rows_; }
    [[nodiscard]] std::size_t cols() const noexcept { return cols_; }
    [[nodiscard]] std::size_t size() const noexcept { return rows_ * cols_; }
    [[nodiscard]] bool empty() const noexcept { return rows_ == 0 || cols_ == 0; }
    [[nodiscard]] bool is_square() const noexcept { return rows_ == cols_; }

    void reshape(std::size_t m, std::size_t n);
    [[nodiscard]] Vec<T> flatten() const;

    T&       operator()(std::size_t i, std::size_t j) noexcept       { return data_[i * cols_ + j]; }
    const T& operator()(std::size_t i, std::size_t j) const noexcept { return data_[i * cols_ + j]; }
    T&       at(std::size_t i, std::size_t j);
    const T& at(std::size_t i, std::size_t j) const;

    T*       data() noexcept       { return data_; }
    const T* data() const noexcept { return data_; }
    T*       row_data(std::size_t i) noexcept       { return data_ + i * cols_; }
    const T* row_data(std::size_t i) const noexcept { return data_ + i * cols_; }
    T*       begin() noexcept       { return data_; }
    T*       end() noexcept         { return data_ + size(); }
    const T* begin() const noexcept { return data_; }
    const T* end() const noexcept   { return data_ + size(); }

    void fill(T value) noexcept;
    void scale(T alpha) noexcept;
    Matrix& operator+=(const Matrix& other);
    Matrix& operator-=(const Matrix& other);
    Matrix& operator*=(T alpha) noexcept;
    Matrix& operator/=(T alpha) noexcept;

    template <typename F>
    Matrix& apply(F f) {
        const std::size_t n = size();
        for (std::size_t k = 0; k < n; ++k) data_[k] = static_cast<T>(f(data_[k]));
        return *this;
    }

    [[nodiscard]] Matrix transposed() const;
    void transpose_in_place();

    [[nodiscard]] T trace() const;

    [[nodiscard]] Vec<T> diagonal(std::ptrdiff_t k = 0) const;
    void set_diagonal(const Vec<T>& d, std::ptrdiff_t k = 0);

    void swap_rows(std::size_t i, std::size_t j) noexcept;
    void scale_row(std::size_t i, T alpha) noexcept;
    void add_row_multiple(std::size_t target, std::size_t source, T alpha) noexcept;

    [[nodiscard]] T sum() const noexcept;
    [[nodiscard]] T mean() const;
    [[nodiscard]] T min() const;
    [[nodiscard]] T max() const;
    [[nodiscard]] T max_abs() const;
    [[nodiscard]] bool all_finite() const noexcept;

    [[nodiscard]] Vec<T> sum_rows() const;
    [[nodiscard]] Vec<T> sum_cols() const;
    [[nodiscard]] Vec<T> mean_rows() const;
    [[nodiscard]] Vec<T> mean_cols() const;

private:
    T* data_ = nullptr;
    std::size_t rows_ = 0;
    std::size_t cols_ = 0;
};

template <typename T> [[nodiscard]] Matrix<T> operator+(const Matrix<T>& a, const Matrix<T>& b);
template <typename T> [[nodiscard]] Matrix<T> operator-(const Matrix<T>& a, const Matrix<T>& b);
template <typename T> [[nodiscard]] Matrix<T> operator-(const Matrix<T>& a);
template <typename T> [[nodiscard]] Matrix<T> operator*(const Matrix<T>& a, non_deduced_t<T> alpha);
template <typename T> [[nodiscard]] Matrix<T> operator*(non_deduced_t<T> alpha, const Matrix<T>& a);
template <typename T> [[nodiscard]] Matrix<T> operator/(const Matrix<T>& a, non_deduced_t<T> alpha);

template <typename T> [[nodiscard]] inline Matrix<T> operator+(Matrix<T>&& a, const Matrix<T>& b) { a += b;        return std::move(a); }
template <typename T> [[nodiscard]] inline Matrix<T> operator-(Matrix<T>&& a, const Matrix<T>& b) { a -= b;        return std::move(a); }
template <typename T> [[nodiscard]] inline Matrix<T> operator-(Matrix<T>&& a)                     { a.scale(T{-1}); return std::move(a); }
template <typename T> [[nodiscard]] inline Matrix<T> operator*(Matrix<T>&& a, non_deduced_t<T> alpha) { a *= alpha; return std::move(a); }
template <typename T> [[nodiscard]] inline Matrix<T> operator*(non_deduced_t<T> alpha, Matrix<T>&& a) { a *= alpha; return std::move(a); }
template <typename T> [[nodiscard]] inline Matrix<T> operator/(Matrix<T>&& a, non_deduced_t<T> alpha) { a /= alpha; return std::move(a); }

template <typename T>
[[nodiscard]] Matrix<T> hadamard(const Matrix<T>& A, const Matrix<T>& B);

template <typename T> void add_to_rows(Matrix<T>& A, const Vec<T>& v);
template <typename T> void subtract_from_rows(Matrix<T>& A, const Vec<T>& v);

template <typename T> Vec<T> centre_columns(Matrix<T>& A);

template <typename T>
[[nodiscard]] bool allclose(const Matrix<T>& A, const Matrix<T>& B,
                            non_deduced_t<T> rtol = default_tolerance<T>(),
                            non_deduced_t<T> atol = default_tolerance<T>()) noexcept;

template <typename T>
[[nodiscard]] std::string to_string(const Matrix<T>& A,
                                    int digits = std::numeric_limits<T>::digits10);

template <typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& A);

extern template class Matrix<float>;
extern template class Matrix<double>;

using Matrixf = Matrix<float>;
using Matrixd = Matrix<double>;

}
