#include "yatrices/matrix.hpp"

#include "yatrices/errors.hpp"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "internal.hpp"

namespace yatrices {
namespace {

using detail::aligned;
using detail::allocate;
using detail::checked_count;
using detail::map_kernel;
using detail::release;
using detail::sum_map_kernel;
using detail::update_kernel;
using detail::zip_kernel;

constexpr std::size_t kTile = 32;

template <Scalar T>
void check_same_shape(const char* who, const Matrix<T>& a, const Matrix<T>& b) {
    if (a.rows() != b.rows() || a.cols() != b.cols()) {
        throw ShapeMismatchError(who, a.rows(), a.cols(), b.rows(), b.cols(),
                                 "both matrices must have the same shape");
    }
}

struct DiagonalSpan {
    std::size_t start;
    std::size_t length;
};

DiagonalSpan diagonal_span(std::size_t m, std::size_t n, std::ptrdiff_t k) noexcept {
    if (k >= 0) {
        const auto c = static_cast<std::size_t>(k);
        if (c >= n) return {0, 0};
        return {c, std::min(m, n - c)};
    }
    const std::size_t r = static_cast<std::size_t>(-(k + 1)) + 1;
    if (r >= m) return {0, 0};
    return {r * n, std::min(m - r, n)};
}

}

template <Scalar T>
Matrix<T>::Matrix(std::size_t rows, std::size_t cols)
    : data_(allocate<T>(checked_count<T>(rows, cols))), rows_(rows), cols_(cols) {
    if (size() != 0) std::memset(data_, 0, size() * sizeof(T));
}

template <Scalar T>
Matrix<T>::Matrix(std::size_t rows, std::size_t cols, T value)
    : data_(allocate<T>(checked_count<T>(rows, cols))), rows_(rows), cols_(cols) {
    std::fill(data_, data_ + size(), value);
}

template <Scalar T>
Matrix<T>::Matrix(std::size_t rows, std::size_t cols, UninitialisedTag)
    : data_(allocate<T>(checked_count<T>(rows, cols))), rows_(rows), cols_(cols) {}

namespace {

template <Scalar T>
std::size_t checked_cols(std::initializer_list<std::initializer_list<T>> rows) {
    if (rows.size() == 0) return 0;
    const std::size_t n = rows.begin()->size();
    std::size_t i = 0;
    for (const auto& r : rows) {
        if (r.size() != n) throw RaggedRowsError("Matrix({{...}})", i, r.size(), n);
        ++i;
    }
    return n;
}

}

template <Scalar T>
Matrix<T>::Matrix(std::initializer_list<std::initializer_list<T>> rows)
    : Matrix(rows.size(), checked_cols(rows), uninitialised) {
    T* out = data_;
    for (const auto& r : rows) out = std::copy(r.begin(), r.end(), out);
}

template <Scalar T>
Matrix<T>::Matrix(const Matrix& other)
    : data_(allocate<T>(other.size())), rows_(other.rows_), cols_(other.cols_) {
    if (size() != 0) std::memcpy(data_, other.data_, size() * sizeof(T));
}

template <Scalar T>
Matrix<T>::Matrix(Matrix&& other) noexcept
    : data_(std::exchange(other.data_, nullptr)),
      rows_(std::exchange(other.rows_, 0)),
      cols_(std::exchange(other.cols_, 0)) {}

template <Scalar T>
Matrix<T>& Matrix<T>::operator=(const Matrix& other) {
    if (this == &other) return *this;
    if (size() == other.size()) {
        if (size() != 0) std::memcpy(data_, other.data_, size() * sizeof(T));
        rows_ = other.rows_;
        cols_ = other.cols_;
    } else {
        Matrix copy(other);
        swap(copy);
    }
    return *this;
}

template <Scalar T>
Matrix<T>& Matrix<T>::operator=(Matrix&& other) noexcept {
    if (this != &other) {
        release(data_);
        data_ = std::exchange(other.data_, nullptr);
        rows_ = std::exchange(other.rows_, 0);
        cols_ = std::exchange(other.cols_, 0);
    }
    return *this;
}

template <Scalar T>
Matrix<T>::~Matrix() {
    release(data_);
}

template <Scalar T>
void Matrix<T>::swap(Matrix& other) noexcept {
    std::swap(data_, other.data_);
    std::swap(rows_, other.rows_);
    std::swap(cols_, other.cols_);
}

template <Scalar T>
void Matrix<T>::reshape(std::size_t m, std::size_t n) {
    if (checked_count<T>(m, n) != size()) {
        throw ShapeMismatchError("Matrix::reshape", rows_, cols_, m, n,
                                 "a reshape must keep the number of elements");
    }
    rows_ = m;
    cols_ = n;
}

template <Scalar T>
Vec<T> Matrix<T>::flatten() const {
    Vec<T> v(size(), uninitialised);
    if (size() != 0) std::memcpy(v.data(), data_, size() * sizeof(T));
    return v;
}

template <Scalar T>
T& Matrix<T>::at(std::size_t i, std::size_t j) {
    if (i >= rows_) throw IndexError("Matrix::at", "row", i, rows_);
    if (j >= cols_) throw IndexError("Matrix::at", "column", j, cols_);
    return data_[i * cols_ + j];
}

template <Scalar T>
const T& Matrix<T>::at(std::size_t i, std::size_t j) const {
    if (i >= rows_) throw IndexError("Matrix::at", "row", i, rows_);
    if (j >= cols_) throw IndexError("Matrix::at", "column", j, cols_);
    return data_[i * cols_ + j];
}

template <Scalar T>
void Matrix<T>::fill(T value) noexcept {
    if (size() != 0) std::fill(aligned(data_), aligned(data_) + size(), value);
}

template <Scalar T>
void Matrix<T>::scale(T alpha) noexcept {
    T* YATRICES_RESTRICT p = aligned(data_);
    const std::size_t n = size();
    for (std::size_t k = 0; k < n; ++k) p[k] *= alpha;
}

template <Scalar T>
Matrix<T>& Matrix<T>::operator+=(const Matrix& other) {
    check_same_shape("Matrix::operator+=", *this, other);
    if (empty()) return *this;
    if (this == &other) {
        scale(T{2});
        return *this;
    }
    update_kernel(aligned(data_), aligned(other.data_), size(), [](T p, T q) { return p + q; });
    return *this;
}

template <Scalar T>
Matrix<T>& Matrix<T>::operator-=(const Matrix& other) {
    check_same_shape("Matrix::operator-=", *this, other);
    if (empty()) return *this;
    if (this == &other) {
        for (std::size_t k = 0; k < size(); ++k) data_[k] -= data_[k];
        return *this;
    }
    update_kernel(aligned(data_), aligned(other.data_), size(), [](T p, T q) { return p - q; });
    return *this;
}

template <Scalar T>
Matrix<T>& Matrix<T>::operator*=(T alpha) noexcept {
    scale(alpha);
    return *this;
}

template <Scalar T>
Matrix<T>& Matrix<T>::operator/=(T alpha) noexcept {
    T* YATRICES_RESTRICT p = aligned(data_);
    const std::size_t n = size();
    for (std::size_t k = 0; k < n; ++k) p[k] /= alpha;
    return *this;
}

template <Scalar T>
Matrix<T> Matrix<T>::transposed() const {
    Matrix out(cols_, rows_, uninitialised);
    const T* YATRICES_RESTRICT a = data_;
    T* YATRICES_RESTRICT o = out.data_;
    const std::size_t m = rows_;
    const std::size_t n = cols_;
    for (std::size_t ii = 0; ii < m; ii += kTile) {
        const std::size_t i_end = std::min(ii + kTile, m);
        for (std::size_t jj = 0; jj < n; jj += kTile) {
            const std::size_t j_end = std::min(jj + kTile, n);
            for (std::size_t j = jj; j < j_end; ++j) {
                for (std::size_t i = ii; i < i_end; ++i) o[j * m + i] = a[i * n + j];
            }
        }
    }
    return out;
}

template <Scalar T>
void Matrix<T>::transpose_in_place() {
    if (!is_square()) throw NotSquareError("Matrix::transpose_in_place", rows_, cols_);
    const std::size_t n = rows_;
    T* a = data_;
    for (std::size_t ii = 0; ii < n; ii += kTile) {
        const std::size_t i_end = std::min(ii + kTile, n);
        for (std::size_t jj = ii; jj < n; jj += kTile) {
            const std::size_t j_end = std::min(jj + kTile, n);
            for (std::size_t i = ii; i < i_end; ++i) {
                for (std::size_t j = (ii == jj ? i + 1 : jj); j < j_end; ++j) {
                    std::swap(a[i * n + j], a[j * n + i]);
                }
            }
        }
    }
}

template <Scalar T>
T Matrix<T>::trace() const {
    if (!is_square()) throw NotSquareError("Matrix::trace", rows_, cols_);
    T t{0};
    for (std::size_t k = 0; k < size(); k += cols_ + 1) t += data_[k];
    return t;
}

template <Scalar T>
Vec<T> Matrix<T>::diagonal(std::ptrdiff_t k) const {
    const DiagonalSpan s = diagonal_span(rows_, cols_, k);
    Vec<T> d(s.length, uninitialised);
    for (std::size_t t = 0; t < s.length; ++t) d[t] = data_[s.start + t * (cols_ + 1)];
    return d;
}

template <Scalar T>
void Matrix<T>::set_diagonal(const Vec<T>& d, std::ptrdiff_t k) {
    const DiagonalSpan s = diagonal_span(rows_, cols_, k);
    if (d.size() != s.length) throw WrongSizeError("Matrix::set_diagonal", d.size(), s.length);
    for (std::size_t t = 0; t < s.length; ++t) data_[s.start + t * (cols_ + 1)] = d[t];
}

template <Scalar T>
void Matrix<T>::swap_rows(std::size_t i, std::size_t j) noexcept {
    if (i == j) return;
    std::swap_ranges(row_data(i), row_data(i) + cols_, row_data(j));
}

template <Scalar T>
void Matrix<T>::scale_row(std::size_t i, T alpha) noexcept {
    T* YATRICES_RESTRICT r = row_data(i);
    for (std::size_t j = 0; j < cols_; ++j) r[j] *= alpha;
}

template <Scalar T>
void Matrix<T>::add_row_multiple(std::size_t target, std::size_t source, T alpha) noexcept {
    if (target == source) {
        scale_row(target, T{1} + alpha);
        return;
    }
    detail::axpy_kernel(row_data(target), row_data(source), alpha, cols_);
}

template <Scalar T>
T Matrix<T>::sum() const noexcept {
    if (size() == 0) return T{0};
    return sum_map_kernel(aligned(data_), size(), [](T v) { return v; });
}

template <Scalar T>
T Matrix<T>::mean() const {
    if (size() == 0) throw EmptyMatrixError("Matrix::mean");
    return sum() / static_cast<T>(size());
}

template <Scalar T>
T Matrix<T>::min() const {
    if (size() == 0) throw EmptyMatrixError("Matrix::min");
    return *std::min_element(data_, data_ + size());
}

template <Scalar T>
T Matrix<T>::max() const {
    if (size() == 0) throw EmptyMatrixError("Matrix::max");
    return *std::max_element(data_, data_ + size());
}

template <Scalar T>
T Matrix<T>::max_abs() const {
    if (size() == 0) throw EmptyMatrixError("Matrix::max_abs");
    T best{0};
    for (std::size_t k = 0; k < size(); ++k) {
        const T a = std::fabs(data_[k]);
        if (std::isnan(a)) return a;
        if (a > best) best = a;
    }
    return best;
}

template <Scalar T>
bool Matrix<T>::all_finite() const noexcept {
    for (std::size_t k = 0; k < size(); ++k) {
        if (!std::isfinite(data_[k])) return false;
    }
    return true;
}

template <Scalar T>
Vec<T> Matrix<T>::sum_rows() const {
    Vec<T> out(rows_, uninitialised);
    for (std::size_t i = 0; i < rows_; ++i) {
        out[i] = sum_map_kernel(row_data(i), cols_, [](T v) { return v; });
    }
    return out;
}

template <Scalar T>
Vec<T> Matrix<T>::sum_cols() const {
    Vec<T> out(cols_);
    for (std::size_t i = 0; i < rows_; ++i) {
        update_kernel(out.data(), row_data(i), cols_, [](T p, T q) { return p + q; });
    }
    return out;
}

template <Scalar T>
Vec<T> Matrix<T>::mean_rows() const {
    if (cols_ == 0) throw EmptyMatrixError("Matrix::mean_rows");
    Vec<T> out = sum_rows();
    out /= static_cast<T>(cols_);
    return out;
}

template <Scalar T>
Vec<T> Matrix<T>::mean_cols() const {
    if (rows_ == 0) throw EmptyMatrixError("Matrix::mean_cols");
    Vec<T> out = sum_cols();
    out /= static_cast<T>(rows_);
    return out;
}

template <Scalar T>
Matrix<T> operator+(const Matrix<T>& a, const Matrix<T>& b) {
    check_same_shape("operator+", a, b);
    Matrix<T> out(a.rows(), a.cols(), uninitialised);
    if (!a.empty()) {
        zip_kernel(aligned(out.data()), aligned(a.data()), aligned(b.data()), a.size(),
                   [](T p, T q) { return p + q; });
    }
    return out;
}

template <Scalar T>
Matrix<T> operator-(const Matrix<T>& a, const Matrix<T>& b) {
    check_same_shape("operator-", a, b);
    Matrix<T> out(a.rows(), a.cols(), uninitialised);
    if (!a.empty()) {
        zip_kernel(aligned(out.data()), aligned(a.data()), aligned(b.data()), a.size(),
                   [](T p, T q) { return p - q; });
    }
    return out;
}

template <Scalar T>
Matrix<T> operator-(const Matrix<T>& a) {
    Matrix<T> out(a.rows(), a.cols(), uninitialised);
    if (!a.empty()) {
        map_kernel(aligned(out.data()), aligned(a.data()), a.size(), [](T p) { return -p; });
    }
    return out;
}

template <Scalar T>
Matrix<T> operator*(const Matrix<T>& a, non_deduced_t<T> alpha) {
    Matrix<T> out(a.rows(), a.cols(), uninitialised);
    if (!a.empty()) {
        map_kernel(aligned(out.data()), aligned(a.data()), a.size(),
                   [alpha](T p) { return p * alpha; });
    }
    return out;
}

template <Scalar T>
Matrix<T> operator*(non_deduced_t<T> alpha, const Matrix<T>& a) { return a * alpha; }

template <Scalar T>
Matrix<T> operator/(const Matrix<T>& a, non_deduced_t<T> alpha) {
    Matrix<T> out(a.rows(), a.cols(), uninitialised);
    if (!a.empty()) {
        map_kernel(aligned(out.data()), aligned(a.data()), a.size(),
                   [alpha](T p) { return p / alpha; });
    }
    return out;
}

template <Scalar T>
Matrix<T> hadamard(const Matrix<T>& A, const Matrix<T>& B) {
    check_same_shape("hadamard", A, B);
    Matrix<T> out(A.rows(), A.cols(), uninitialised);
    if (!A.empty()) {
        zip_kernel(aligned(out.data()), aligned(A.data()), aligned(B.data()), A.size(),
                   [](T p, T q) { return p * q; });
    }
    return out;
}

template <Scalar T>
void add_to_rows(Matrix<T>& A, const Vec<T>& v) {
    if (v.size() != A.cols()) throw SizeMismatchError("add_to_rows", A.cols(), v.size());
    for (std::size_t i = 0; i < A.rows(); ++i) {
        update_kernel(A.row_data(i), v.data(), A.cols(), [](T p, T q) { return p + q; });
    }
}

template <Scalar T>
void subtract_from_rows(Matrix<T>& A, const Vec<T>& v) {
    if (v.size() != A.cols()) throw SizeMismatchError("subtract_from_rows", A.cols(), v.size());
    for (std::size_t i = 0; i < A.rows(); ++i) {
        update_kernel(A.row_data(i), v.data(), A.cols(), [](T p, T q) { return p - q; });
    }
}

template <Scalar T>
Vec<T> centre_columns(Matrix<T>& A) {
    if (A.rows() == 0) throw EmptyMatrixError("centre_columns");
    Vec<T> means = A.mean_cols();
    subtract_from_rows(A, means);
    return means;
}

template <Scalar T>
bool allclose(const Matrix<T>& A, const Matrix<T>& B, non_deduced_t<T> rtol,
              non_deduced_t<T> atol) noexcept {
    if (A.rows() != B.rows() || A.cols() != B.cols()) return false;
    const T* a = A.data();
    const T* b = B.data();
    for (std::size_t k = 0; k < A.size(); ++k) {
        if (!approx_equal(a[k], b[k], rtol, atol)) return false;
    }
    return true;
}

template <Scalar T>
std::string to_string(const Matrix<T>& A, int digits) {
    if (digits < 1) digits = 1;
    if (A.rows() == 0) return "[]";
    std::vector<std::string> cells(A.size());
    std::vector<std::size_t> width(A.cols(), 0);
    char buf[64];
    for (std::size_t i = 0; i < A.rows(); ++i) {
        for (std::size_t j = 0; j < A.cols(); ++j) {
            std::snprintf(buf, sizeof buf, "%.*g", digits, static_cast<double>(A(i, j)));
            cells[i * A.cols() + j] = buf;
            width[j] = std::max(width[j], cells[i * A.cols() + j].size());
        }
    }
    std::string out = "[";
    for (std::size_t i = 0; i < A.rows(); ++i) {
        if (i != 0) out += ",\n ";
        out += "[";
        for (std::size_t j = 0; j < A.cols(); ++j) {
            if (j != 0) out += ", ";
            const std::string& c = cells[i * A.cols() + j];
            out.append(width[j] - c.size(), ' ');
            out += c;
        }
        out += "]";
    }
    return out + "]";
}

template <Scalar T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& A) {
    return os << to_string(A);
}

template class Matrix<float>;
template class Matrix<double>;

#define YATRICES_INSTANTIATE_MATRIX(T)                                                        \
    template Matrix<T> operator+<T>(const Matrix<T>&, const Matrix<T>&);                      \
    template Matrix<T> operator-<T>(const Matrix<T>&, const Matrix<T>&);                      \
    template Matrix<T> operator-<T>(const Matrix<T>&);                                        \
    template Matrix<T> operator*<T>(const Matrix<T>&, T);                                     \
    template Matrix<T> operator*<T>(T, const Matrix<T>&);                                     \
    template Matrix<T> operator/<T>(const Matrix<T>&, T);                                     \
    template Matrix<T> hadamard<T>(const Matrix<T>&, const Matrix<T>&);                       \
    template void add_to_rows<T>(Matrix<T>&, const Vec<T>&);                                  \
    template void subtract_from_rows<T>(Matrix<T>&, const Vec<T>&);                           \
    template Vec<T> centre_columns<T>(Matrix<T>&);                                            \
    template bool allclose<T>(const Matrix<T>&, const Matrix<T>&, T, T) noexcept;             \
    template std::string to_string<T>(const Matrix<T>&, int);                                 \
    template std::ostream& operator<< <T>(std::ostream&, const Matrix<T>&);

YATRICES_INSTANTIATE_MATRIX(float)
YATRICES_INSTANTIATE_MATRIX(double)

#undef YATRICES_INSTANTIATE_MATRIX

}
