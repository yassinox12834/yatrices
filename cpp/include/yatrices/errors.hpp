#pragma once

#include <cstddef>
#include <new>
#include <stdexcept>
#include <string>

// Yatrices error types.
//
// Each one derives from the standard exception it refines, so code that
// catches std::out_of_range or std::invalid_argument still catches ours.
// Every message starts with "yatrices:", names the function that failed,
// and says what to do about it.
namespace yatrices {

// An index past the end of a vector, e.g. v.at(7) on a vector of size 3.
class IndexError : public std::out_of_range {
public:
    IndexError(const char* where, std::size_t index, std::size_t size);
    std::size_t index() const noexcept { return index_; }
    std::size_t size() const noexcept { return size_; }

private:
    std::size_t index_;
    std::size_t size_;
};

// Two operands that must have the same size do not, e.g. dot of 3 and 4.
class SizeMismatchError : public std::invalid_argument {
public:
    SizeMismatchError(const char* where, std::size_t left, std::size_t right);
    std::size_t left() const noexcept { return left_; }
    std::size_t right() const noexcept { return right_; }

private:
    std::size_t left_;
    std::size_t right_;
};

// An operation that has no meaning on an empty vector (a maximum, a mean).
class EmptyVectorError : public std::invalid_argument {
public:
    explicit EmptyVectorError(const char* where);
};

// A vector whose size is wrong for the operation, e.g. cross of two size-4 vectors.
class WrongSizeError : public std::invalid_argument {
public:
    WrongSizeError(const char* where, std::size_t size, std::size_t expected);
};

// An operation that needs a direction, given a vector of zeros (angle, project).
class ZeroVectorError : public std::invalid_argument {
public:
    explicit ZeroVectorError(const char* where);
};

// A size so large that its byte count does not even fit in std::size_t.
class AllocationSizeError : public std::length_error {
public:
    AllocationSizeError(std::size_t count, std::size_t element_bytes);
};

// The size was valid but the computer ran out of memory.
class OutOfMemoryError : public std::bad_alloc {
public:
    OutOfMemoryError(std::size_t count, std::size_t element_bytes);
    const char* what() const noexcept override { return message_.c_str(); }

private:
    std::string message_;
};

}  // namespace yatrices
