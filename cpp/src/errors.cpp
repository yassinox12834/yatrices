#include "yatrices/errors.hpp"

#include <cstdio>
#include <string>

namespace yatrices {
namespace {

std::string str(std::size_t n) { return std::to_string(n); }

// 8000000000 bytes -> "7.45 GiB": readable sizes in messages.
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

std::string mismatch_message(const char* where, std::size_t left, std::size_t right) {
    return std::string("yatrices: ") + where + ": cannot combine a vector of size " +
           str(left) + " with one of size " + str(right) +
           ". Both operands must have the same size.";
}

}  // namespace

IndexError::IndexError(const char* where, std::size_t index, std::size_t size)
    : std::out_of_range(index_message(where, index, size)), index_(index), size_(size) {}

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

AllocationSizeError::AllocationSizeError(std::size_t count, std::size_t element_bytes)
    : std::length_error("yatrices: cannot allocate " + str(count) + " elements of " +
                        str(element_bytes) + " bytes each; the total is larger than any "
                        "computer can address. The size is probably a bug, for example a "
                        "negative number converted to std::size_t.") {}

OutOfMemoryError::OutOfMemoryError(std::size_t count, std::size_t element_bytes)
    : message_("yatrices: not enough memory to allocate " + str(count) + " elements (" +
               human_bytes(static_cast<double>(count) * static_cast<double>(element_bytes)) +
               "). Free some memory, or use float instead of double to halve the size.") {}

}  // namespace yatrices
