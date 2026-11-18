#include "yatrices/errors.hpp"

#include <cstdio>
#include <string>

namespace yatrices {
namespace {

std::string str(std::size_t n) { return std::to_string(n); }

std::string human_bytes(double bytes) {
    const char* units[] = {"bytes", "KiB", "MiB", "GiB", "TiB", "PiB", "EiB"};
    int u = 0;
    while (bytes >= 1024.0 && u < 6) {
        bytes /= 1024.0;
        ++u;
    }
    char buf[32];
    std::snprintf(buf, sizeof buf, u == 0 ? "%.0f %s" : "%.2f %s", bytes, units[u]);
    return buf;
}

std::string index_message(const char* where, std::size_t index, std::size_t size) {
    std::string m = std::string("yatrices: ") + where + ": index " + str(index) +
                    " is out of range";
    if (size == 0) return m + ", because the vector is empty, so no index is valid.";
    return m + " for a vector of size " + str(size) + ". Valid indices are 0 to " +
           str(size - 1) + "; remember that counting starts at 0.";
}

std::string axis_index_message(const char* where, const char* axis, std::size_t index,
                               std::size_t size) {
    std::string m = std::string("yatrices: ") + where + ": " + axis + " index " + str(index) +
                    " is out of range";
    if (size == 0) return m + ", because the matrix has no " + axis + "s.";
    return m + " for a matrix with " + str(size) + " " + axis + (size == 1 ? "" : "s") +
           ". Valid " + axis + " indices are 0 to " + str(size - 1) + ".";
}

std::string shape(std::size_t r, std::size_t c) { return str(r) + " x " + str(c); }

std::string mismatch_message(const char* where, std::size_t left, std::size_t right) {
    return std::string("yatrices: ") + where + ": cannot combine a vector of size " +
           str(left) + " with one of size " + str(right) +
           ". Both operands must have the same size.";
}

}

IndexError::IndexError(const char* where, std::size_t index, std::size_t size)
    : std::out_of_range(index_message(where, index, size)), index_(index), size_(size) {}

IndexError::IndexError(const char* where, const char* axis, std::size_t index,
                       std::size_t size)
    : std::out_of_range(axis_index_message(where, axis, index, size)),
      index_(index),
      size_(size) {}

SizeMismatchError::SizeMismatchError(const char* where, std::size_t left, std::size_t right)
    : std::invalid_argument(mismatch_message(where, left, right)), left_(left), right_(right) {}

EmptyVectorError::EmptyVectorError(const char* where)
    : std::invalid_argument(std::string("yatrices: ") + where +
                            ": this operation is not defined for an empty vector. "
                            "Check that the vector has been filled before using it.") {}

WrongSizeError::WrongSizeError(const char* where, std::size_t size, std::size_t expected)
    : std::invalid_argument(std::string("yatrices: ") + where + ": needs vectors of size " +
                            str(expected) + ", got size " + str(size) + ".") {}

ZeroVectorError::ZeroVectorError(const char* where)
    : std::invalid_argument(std::string("yatrices: ") + where +
                            ": a vector of zeros has no direction, so the result is "
                            "undefined. Check the vector before calling this.") {}

ShapeMismatchError::ShapeMismatchError(const char* where, std::size_t left_rows,
                                       std::size_t left_cols, std::size_t right_rows,
                                       std::size_t right_cols, const char* rule)
    : std::invalid_argument(std::string("yatrices: ") + where + ": shapes " +
                            shape(left_rows, left_cols) + " and " +
                            shape(right_rows, right_cols) + " do not fit together; " + rule +
                            "."),
      left_rows_(left_rows),
      left_cols_(left_cols),
      right_rows_(right_rows),
      right_cols_(right_cols) {}

NotSquareError::NotSquareError(const char* where, std::size_t rows, std::size_t cols)
    : std::invalid_argument(std::string("yatrices: ") + where +
                            ": needs a square matrix (as many rows as columns), got " +
                            shape(rows, cols) + ".") {}

RaggedRowsError::RaggedRowsError(const char* where, std::size_t row, std::size_t length,
                                 std::size_t expected)
    : std::invalid_argument(std::string("yatrices: ") + where + ": row " + str(row) + " has " +
                            str(length) + " values but row 0 has " + str(expected) +
                            ". Every row of a matrix must have the same length.") {}

EmptyMatrixError::EmptyMatrixError(const char* where)
    : std::invalid_argument(std::string("yatrices: ") + where +
                            ": this operation is not defined for a matrix with no elements. "
                            "Check its shape before using it.") {}

AllocationSizeError::AllocationSizeError(std::size_t rows, std::size_t cols,
                                         std::size_t element_bytes)
    : std::length_error("yatrices: cannot allocate a " + shape(rows, cols) + " matrix of " +
                        str(element_bytes) + "-byte elements; rows * cols is larger than any "
                        "computer can address. One of the sizes is probably a bug, for example "
                        "a negative number converted to std::size_t.") {}

AllocationSizeError::AllocationSizeError(std::size_t count, std::size_t element_bytes)
    : std::length_error("yatrices: cannot allocate " + str(count) + " elements of " +
                        str(element_bytes) + " bytes each; the total is larger than any "
                        "computer can address. The size is probably a bug, for example a "
                        "negative number converted to std::size_t.") {}

OutOfMemoryError::OutOfMemoryError(std::size_t count, std::size_t element_bytes)
    : message_("yatrices: not enough memory to allocate " + str(count) + " elements (" +
               human_bytes(static_cast<double>(count) * static_cast<double>(element_bytes)) +
               "). Free some memory, or use float instead of double to halve the size.") {}

}
