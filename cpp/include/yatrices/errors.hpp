#pragma once

#include <cstddef>
#include <new>
#include <stdexcept>
#include <string>

namespace yatrices {

class IndexError : public std::out_of_range {
public:
    IndexError(const char* where, std::size_t index, std::size_t size);
    IndexError(const char* where, const char* axis, std::size_t index, std::size_t size);
    std::size_t index() const noexcept { return index_; }
    std::size_t size() const noexcept { return size_; }

private:
    std::size_t index_;
    std::size_t size_;
};

class SizeMismatchError : public std::invalid_argument {
public:
    SizeMismatchError(const char* where, std::size_t left, std::size_t right);
    std::size_t left() const noexcept { return left_; }
    std::size_t right() const noexcept { return right_; }

private:
    std::size_t left_;
    std::size_t right_;
};

class EmptyVectorError : public std::invalid_argument {
public:
    explicit EmptyVectorError(const char* where);
};

class WrongSizeError : public std::invalid_argument {
public:
    WrongSizeError(const char* where, std::size_t size, std::size_t expected);
};

class ZeroVectorError : public std::invalid_argument {
public:
    explicit ZeroVectorError(const char* where);
};

class ShapeMismatchError : public std::invalid_argument {
public:
    ShapeMismatchError(const char* where, std::size_t left_rows, std::size_t left_cols,
                       std::size_t right_rows, std::size_t right_cols, const char* rule);
    std::size_t left_rows() const noexcept { return left_rows_; }
    std::size_t left_cols() const noexcept { return left_cols_; }
    std::size_t right_rows() const noexcept { return right_rows_; }
    std::size_t right_cols() const noexcept { return right_cols_; }

private:
    std::size_t left_rows_;
    std::size_t left_cols_;
    std::size_t right_rows_;
    std::size_t right_cols_;
};

class NotSquareError : public std::invalid_argument {
public:
    NotSquareError(const char* where, std::size_t rows, std::size_t cols);
};

class RaggedRowsError : public std::invalid_argument {
public:
    RaggedRowsError(const char* where, std::size_t row, std::size_t length,
                    std::size_t expected);
};

class EmptyMatrixError : public std::invalid_argument {
public:
    explicit EmptyMatrixError(const char* where);
};

class AllocationSizeError : public std::length_error {
public:
    AllocationSizeError(std::size_t count, std::size_t element_bytes);
    AllocationSizeError(std::size_t rows, std::size_t cols, std::size_t element_bytes);
};

class OutOfMemoryError : public std::bad_alloc {
public:
    OutOfMemoryError(std::size_t count, std::size_t element_bytes);
    const char* what() const noexcept override { return message_.c_str(); }

private:
    std::string message_;
};

}
