#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

#include "yatrices/errors.hpp"
#include "yatrices/matrix.hpp"
#include "yatrices/vector.hpp"

namespace py = pybind11;
using yatrices::Matrix;
using yatrices::Vec;

namespace {

PyObject* E_base = nullptr;
PyObject* E_index = nullptr;
PyObject* E_mismatch = nullptr;
PyObject* E_empty = nullptr;
PyObject* E_wrong_size = nullptr;
PyObject* E_zero = nullptr;
PyObject* E_shape = nullptr;
PyObject* E_not_square = nullptr;
PyObject* E_ragged = nullptr;
PyObject* E_empty_matrix = nullptr;
PyObject* E_alloc = nullptr;
PyObject* E_memory = nullptr;

PyObject* make_error(py::module_& m, const char* name, const char* doc, PyObject* builtin) {
    const std::string qualified = std::string("yatricespy.") + name;
    py::object bases = builtin ? py::object(py::make_tuple(py::handle(E_base), py::handle(builtin)))
                               : py::reinterpret_borrow<py::object>(PyExc_Exception);
    PyObject* cls = PyErr_NewExceptionWithDoc(qualified.c_str(), doc, bases.ptr(), nullptr);
    if (cls == nullptr) throw py::error_already_set();
    m.add_object(name, py::handle(cls));
    return cls;
}

void bind_errors(py::module_& m) {
    E_base = make_error(m, "YatricesError", "Base class of every Yatrices error.", nullptr);
    E_index = make_error(m, "IndexError", "An index past the end of a vector.", PyExc_IndexError);
    E_mismatch = make_error(m, "SizeMismatchError",
                            "Two vectors that must have the same size do not.", PyExc_ValueError);
    E_empty = make_error(m, "EmptyVectorError",
                         "An operation with no meaning on an empty vector (mean, min, max).",
                         PyExc_ValueError);
    E_wrong_size = make_error(m, "WrongSizeError",
                              "A vector of the wrong size for the operation (cross needs size 3).",
                              PyExc_ValueError);
    E_zero = make_error(m, "ZeroVectorError",
                        "A vector of zeros where a direction is needed (angle, project).",
                        PyExc_ValueError);
    E_shape = make_error(m, "ShapeMismatchError",
                         "Two matrices whose shapes do not fit the operation.", PyExc_ValueError);
    E_not_square = make_error(m, "NotSquareError",
                              "A matrix that must be square (trace, transpose_in_place) is not.",
                              PyExc_ValueError);
    E_ragged = make_error(m, "RaggedRowsError",
                          "Rows of different lengths given to build a matrix.", PyExc_ValueError);
    E_empty_matrix = make_error(m, "EmptyMatrixError",
                                "An operation with no meaning on an empty matrix (mean, min, max).",
                                PyExc_ValueError);
    E_alloc = make_error(m, "AllocationSizeError",
                         "A size so large that its byte count does not fit in memory addresses.",
                         PyExc_ValueError);
    E_memory = make_error(m, "OutOfMemoryError", "The computer ran out of memory.",
                          PyExc_MemoryError);

    py::register_exception_translator([](std::exception_ptr p) {
        try {
            if (p) std::rethrow_exception(p);
        } catch (const yatrices::IndexError& e) {
            PyErr_SetString(E_index, e.what());
        } catch (const yatrices::SizeMismatchError& e) {
            PyErr_SetString(E_mismatch, e.what());
        } catch (const yatrices::EmptyVectorError& e) {
            PyErr_SetString(E_empty, e.what());
        } catch (const yatrices::WrongSizeError& e) {
            PyErr_SetString(E_wrong_size, e.what());
        } catch (const yatrices::ZeroVectorError& e) {
            PyErr_SetString(E_zero, e.what());
        } catch (const yatrices::ShapeMismatchError& e) {
            PyErr_SetString(E_shape, e.what());
        } catch (const yatrices::NotSquareError& e) {
            PyErr_SetString(E_not_square, e.what());
        } catch (const yatrices::RaggedRowsError& e) {
            PyErr_SetString(E_ragged, e.what());
        } catch (const yatrices::EmptyMatrixError& e) {
            PyErr_SetString(E_empty_matrix, e.what());
        } catch (const yatrices::AllocationSizeError& e) {
            PyErr_SetString(E_alloc, e.what());
        } catch (const yatrices::OutOfMemoryError& e) {
            PyErr_SetString(E_memory, e.what());
        }
    });
}

std::size_t python_index(py::ssize_t i, std::size_t size) {
    const py::ssize_t n = static_cast<py::ssize_t>(size);
    const py::ssize_t j = i < 0 ? i + n : i;
    if (j < 0 || j >= n) {
        std::string msg = "yatrices: Vec[]: index " + std::to_string(i) + " is out of range";
        if (n == 0) {
            msg += ", because the vector is empty, so no index is valid.";
        } else {
            msg += " for a vector of size " + std::to_string(n) + ". Valid indices are " +
                   std::to_string(-n) + " to " + std::to_string(n - 1) + ".";
        }
        PyErr_SetString(E_index, msg.c_str());
        throw py::error_already_set();
    }
    return static_cast<std::size_t>(j);
}

template <typename T>
void bind_vec(py::module_& m, const char* name, const char* dtype) {
    using V = Vec<T>;
    const std::string cls_name = name;

    py::class_<V>(m, name,
                  "Fixed-size vector stored in the C++ engine.\n\n"
                  "Build one from a list (Vec([1, 2, 3])), a size (Vec(3) gives zeros),\n"
                  "or a size and a value (Vec(3, 7.0)).")
        .def(py::init<std::size_t>(), py::arg("n"), "n zeros.")
        .def(py::init<std::size_t, T>(), py::arg("n"), py::arg("value"), "n copies of value.")
        .def(py::init([](const std::vector<T>& values) {
                 V v(values.size(), yatrices::uninitialised);
                 for (std::size_t i = 0; i < values.size(); ++i) v[i] = values[i];
                 return v;
             }),
             py::arg("values"), "A copy of a list (or any sequence) of numbers.")

        .def("size", &V::size, "Number of elements.")
        .def("empty", &V::empty, "True when the vector has no elements.")
        .def("__len__", &V::size)
        .def("__getitem__",
             [](const V& v, py::ssize_t i) { return v[python_index(i, v.size())]; })
        .def("__setitem__",
             [](V& v, py::ssize_t i, T value) { v[python_index(i, v.size())] = value; })
        .def("__iter__",
             [](const V& v) { return py::make_iterator(v.begin(), v.end()); },
             py::keep_alive<0, 1>())
        .def("tolist", [](const V& v) { return std::vector<T>(v.begin(), v.end()); },
             "A plain Python list with the same values.")
        .def("copy", [](const V& v) { return V(v); }, "An independent copy.")
        .def("__copy__", [](const V& v) { return V(v); })
        .def("__deepcopy__", [](const V& v, py::dict) { return V(v); }, py::arg("memo"))

        .def("__repr__", [cls_name](const V& v) { return cls_name + "(" + yatrices::to_string(v) + ")"; })
        .def("__str__", [](const V& v) { return yatrices::to_string(v); })

        .def("fill", &V::fill, py::arg("value"), "Set every element to value.")
        .def("scale", &V::scale, py::arg("alpha"), "Multiply every element by alpha.")
        .def("axpy", &V::axpy, py::arg("alpha"), py::arg("x"), "self += alpha * x.")

        .def("dot", &V::dot, py::arg("other"), "Dot product: sum of self[i] * other[i].")
        .def("__matmul__", &V::dot, py::is_operator())
        .def("norm1", &V::norm1, "Sum of absolute values.")
        .def("norm2", &V::norm2, "Euclidean length, safe against overflow.")
        .def("norm_inf", &V::norm_inf, "Largest absolute value.")
        .def("sum", &V::sum, "Sum of the elements (pairwise, for accuracy).")
        .def("mean", &V::mean, "Average of the elements. Raises EmptyVectorError if empty.")
        .def("min", &V::min, "Smallest element. Raises EmptyVectorError if empty.")
        .def("max", &V::max, "Largest element. Raises EmptyVectorError if empty.")
        .def("all_finite", &V::all_finite, "False if any element is NaN or infinite.")

        .def("__add__", [](const V& a, const V& b) { return a + b; }, py::is_operator())
        .def("__sub__", [](const V& a, const V& b) { return a - b; }, py::is_operator())
        .def("__neg__", [](const V& a) { return -a; })
        .def("__pos__", [](const V& a) { return V(a); })
        .def("__mul__", [](const V& a, T k) { return a * k; }, py::is_operator())
        .def("__rmul__", [](const V& a, T k) { return k * a; }, py::is_operator())
        .def("__truediv__", [](const V& a, T k) { return a / k; }, py::is_operator())
        .def(py::self += py::self)
        .def(py::self -= py::self)
        .def(py::self *= T())
        .def(py::self /= T())

        .def_property_readonly_static("dtype", [dtype](py::object) { return std::string(dtype); })
        .def_property_readonly_static("alignment", [](py::object) { return V::alignment; })
        .def_static("default_tolerance", &yatrices::default_tolerance<T>,
                    "Tolerance used by allclose when none is given.");

    m.def("hadamard", [](const V& x, const V& y) { return yatrices::hadamard(x, y); },
          py::arg("x"), py::arg("y"), "Element-wise product: x[i] * y[i].");
    m.def("cross", [](const V& x, const V& y) { return yatrices::cross(x, y); },
          py::arg("x"), py::arg("y"), "Cross product of two vectors of size 3.");
    m.def("angle", [](const V& x, const V& y) { return yatrices::angle(x, y); },
          py::arg("x"), py::arg("y"), "Angle between x and y, in radians.");
    m.def("project", [](const V& v, const V& onto) { return yatrices::project(v, onto); },
          py::arg("v"), py::arg("onto"), "Projection of v onto the direction of onto.");
    m.def("allclose",
          [](const V& x, const V& y, T rtol, T atol) { return yatrices::allclose(x, y, rtol, atol); },
          py::arg("x"), py::arg("y"), py::arg("rtol") = yatrices::default_tolerance<T>(),
          py::arg("atol") = yatrices::default_tolerance<T>(),
          "True when every pair of elements is approximately equal.");
}


std::size_t python_axis_index(py::ssize_t i, std::size_t size, const char* where,
                              const char* axis) {
    const py::ssize_t n = static_cast<py::ssize_t>(size);
    const py::ssize_t j = i < 0 ? i + n : i;
    if (j < 0 || j >= n) {
        std::string msg = std::string("yatrices: ") + where + ": " + axis + " index " +
                          std::to_string(i) + " is out of range";
        if (n == 0) {
            msg += std::string(", because the matrix has no ") + axis + "s.";
        } else {
            msg += " for a matrix with " + std::to_string(n) + " " + axis + (n == 1 ? "" : "s") +
                   ". Valid " + axis + " indices are " + std::to_string(-n) + " to " +
                   std::to_string(n - 1) + ".";
        }
        PyErr_SetString(E_index, msg.c_str());
        throw py::error_already_set();
    }
    return static_cast<std::size_t>(j);
}

template <typename T>
Matrix<T> matrix_from_rows(const std::vector<std::vector<T>>& rows) {
    if (rows.empty()) return Matrix<T>();
    const std::size_t n = rows[0].size();
    for (std::size_t i = 0; i < rows.size(); ++i) {
        if (rows[i].size() != n) {
            throw yatrices::RaggedRowsError("Matrix([[...]])", i, rows[i].size(), n);
        }
    }
    Matrix<T> A(rows.size(), n, yatrices::uninitialised);
    for (std::size_t i = 0; i < rows.size(); ++i) {
        for (std::size_t j = 0; j < n; ++j) A(i, j) = rows[i][j];
    }
    return A;
}

template <typename T>
std::vector<std::vector<T>> matrix_to_rows(const Matrix<T>& A) {
    std::vector<std::vector<T>> rows(A.rows());
    for (std::size_t i = 0; i < A.rows(); ++i) {
        rows[i].assign(A.row_data(i), A.row_data(i) + A.cols());
    }
    return rows;
}

std::string indent_lines(const std::string& text, std::size_t spaces) {
    std::string out;
    for (char c : text) {
        out += c;
        if (c == '\n') out.append(spaces, ' ');
    }
    return out;
}

template <typename T>
void bind_matrix(py::module_& m, const char* name, const char* dtype) {
    using M = Matrix<T>;
    using V = Vec<T>;
    const std::string cls_name = name;

    py::class_<M>(m, name,
                  "Dense row-major matrix stored in the C++ engine.\n\n"
                  "Build one from a list of rows (Matrix([[1, 2], [3, 4]])), a shape\n"
                  "(Matrix(2, 3) gives zeros), or a shape and a value (Matrix(2, 3, 7.0)).")
        .def(py::init<std::size_t, std::size_t>(), py::arg("rows"), py::arg("cols"),
             "rows x cols zeros.")
        .def(py::init<std::size_t, std::size_t, T>(), py::arg("rows"), py::arg("cols"),
             py::arg("value"), "rows x cols copies of value.")
        .def(py::init(&matrix_from_rows<T>), py::arg("values"),
             "A copy of a list of rows. Every row must have the same length.")

        .def("rows", &M::rows, "Number of rows.")
        .def("cols", &M::cols, "Number of columns.")
        .def_property_readonly("shape", [](const M& A) { return std::make_pair(A.rows(), A.cols()); },
                               "(rows, cols), like NumPy's shape.")
        .def("size", &M::size, "Number of elements, rows * cols.")
        .def("empty", &M::empty, "True when the matrix has no elements.")
        .def("is_square", &M::is_square, "True when rows == cols.")
        .def("__len__", &M::rows)

        .def("__getitem__",
             [](const M& A, std::pair<py::ssize_t, py::ssize_t> ij) {
                 const std::size_t i = python_axis_index(ij.first, A.rows(), "Matrix[]", "row");
                 const std::size_t j = python_axis_index(ij.second, A.cols(), "Matrix[]", "column");
                 return A(i, j);
             })
        .def("__setitem__",
             [](M& A, std::pair<py::ssize_t, py::ssize_t> ij, T value) {
                 const std::size_t i = python_axis_index(ij.first, A.rows(), "Matrix[]", "row");
                 const std::size_t j = python_axis_index(ij.second, A.cols(), "Matrix[]", "column");
                 A(i, j) = value;
             })
        .def("tolist", &matrix_to_rows<T>, "A plain Python list of rows.")
        .def("flatten", &M::flatten, "Every element, row after row, as a vector.")
        .def("reshape", &M::reshape, py::arg("rows"), py::arg("cols"),
             "Change the shape in place, keeping rows * cols the same.")
        .def("copy", [](const M& A) { return M(A); }, "An independent copy.")
        .def("__copy__", [](const M& A) { return M(A); })
        .def("__deepcopy__", [](const M& A, py::dict) { return M(A); }, py::arg("memo"))

        .def("__repr__",
             [cls_name](const M& A) {
                 return cls_name + "(" + indent_lines(yatrices::to_string(A), cls_name.size() + 1) +
                        ")";
             })
        .def("__str__", [](const M& A) { return yatrices::to_string(A); })

        .def("fill", &M::fill, py::arg("value"), "Set every element to value.")
        .def("scale", &M::scale, py::arg("alpha"), "Multiply every element by alpha.")
        .def("apply",
             [](M& A, const py::function& f) -> M& {
                 return A.apply([&f](T x) { return f(x).template cast<T>(); });
             },
             py::arg("f"), py::return_value_policy::reference_internal,
             "Replace every element x by f(x), in place. Returns the matrix itself.")

        .def("transposed", &M::transposed, "A new matrix with rows and columns swapped.")
        .def_property_readonly("T", &M::transposed, "Same as transposed(), like NumPy's .T.")
        .def("transpose_in_place", &M::transpose_in_place,
             "Swap rows and columns without a copy. Square matrices only.")
        .def("trace", &M::trace, "Sum of the main diagonal. Square matrices only.")
        .def("diagonal", &M::diagonal, py::arg("k") = 0,
             "Diagonal k as a vector: 0 is the main one, k > 0 above it, k < 0 below it.")
        .def("set_diagonal", &M::set_diagonal, py::arg("d"), py::arg("k") = 0,
             "Overwrite diagonal k with the values of d.")

        .def("swap_rows",
             [](M& A, py::ssize_t i, py::ssize_t j) {
                 A.swap_rows(python_axis_index(i, A.rows(), "Matrix.swap_rows", "row"),
                             python_axis_index(j, A.rows(), "Matrix.swap_rows", "row"));
             },
             py::arg("i"), py::arg("j"), "Exchange rows i and j.")
        .def("scale_row",
             [](M& A, py::ssize_t i, T alpha) {
                 A.scale_row(python_axis_index(i, A.rows(), "Matrix.scale_row", "row"), alpha);
             },
             py::arg("i"), py::arg("alpha"), "Multiply row i by alpha.")
        .def("add_row_multiple",
             [](M& A, py::ssize_t target, py::ssize_t source, T alpha) {
                 A.add_row_multiple(
                     python_axis_index(target, A.rows(), "Matrix.add_row_multiple", "row"),
                     python_axis_index(source, A.rows(), "Matrix.add_row_multiple", "row"), alpha);
             },
             py::arg("target"), py::arg("source"), py::arg("alpha"),
             "row[target] += alpha * row[source].")

        .def("sum", &M::sum, "Sum of every element.")
        .def("mean", &M::mean, "Average of every element. Raises EmptyMatrixError if empty.")
        .def("min", &M::min, "Smallest element. Raises EmptyMatrixError if empty.")
        .def("max", &M::max, "Largest element. Raises EmptyMatrixError if empty.")
        .def("max_abs", &M::max_abs, "Largest absolute value. Raises EmptyMatrixError if empty.")
        .def("all_finite", &M::all_finite, "False if any element is NaN or infinite.")
        .def("sum_rows", &M::sum_rows, "One sum per row, as a vector of size rows.")
        .def("sum_cols", &M::sum_cols, "One sum per column, as a vector of size cols.")
        .def("mean_rows", &M::mean_rows, "One mean per row, as a vector of size rows.")
        .def("mean_cols", &M::mean_cols, "One mean per column, as a vector of size cols.")

        .def("__add__", [](const M& a, const M& b) { return a + b; }, py::is_operator())
        .def("__sub__", [](const M& a, const M& b) { return a - b; }, py::is_operator())
        .def("__neg__", [](const M& a) { return -a; })
        .def("__pos__", [](const M& a) { return M(a); })
        .def("__mul__", [](const M& a, T k) { return a * k; }, py::is_operator())
        .def("__rmul__", [](const M& a, T k) { return k * a; }, py::is_operator())
        .def("__truediv__", [](const M& a, T k) { return a / k; }, py::is_operator())
        .def(py::self += py::self)
        .def(py::self -= py::self)
        .def(py::self *= T())
        .def(py::self /= T())

        .def_property_readonly_static("dtype", [dtype](py::object) { return std::string(dtype); })
        .def_property_readonly_static("alignment", [](py::object) { return M::alignment; })
        .def_static("default_tolerance", &yatrices::default_tolerance<T>,
                    "Tolerance used by allclose when none is given.");

    m.def("hadamard", [](const M& A, const M& B) { return yatrices::hadamard(A, B); },
          py::arg("A"), py::arg("B"), "Element-wise product: A[i, j] * B[i, j].");
    m.def("add_to_rows", [](M& A, const V& v) { yatrices::add_to_rows(A, v); }, py::arg("A"),
          py::arg("v"), "Add v to every row of A, in place.");
    m.def("subtract_from_rows", [](M& A, const V& v) { yatrices::subtract_from_rows(A, v); },
          py::arg("A"), py::arg("v"), "Subtract v from every row of A, in place.");
    m.def("centre_columns", [](M& A) { return yatrices::centre_columns(A); }, py::arg("A"),
          "Make every column of A average 0, in place. Returns the column means.");
    m.def("allclose",
          [](const M& A, const M& B, T rtol, T atol) { return yatrices::allclose(A, B, rtol, atol); },
          py::arg("x"), py::arg("y"), py::arg("rtol") = yatrices::default_tolerance<T>(),
          py::arg("atol") = yatrices::default_tolerance<T>(),
          "True when both matrices have the same shape and every pair of elements is close.");
}

}

PYBIND11_MODULE(_yatricespy, m) {
    m.doc() = "yatricespy: Python front end of YatricesEngine";

    bind_errors(m);
    bind_vec<double>(m, "Vec", "float64");
    bind_vec<float>(m, "Vecf", "float32");
    bind_matrix<double>(m, "Matrix", "float64");
    bind_matrix<float>(m, "Matrixf", "float32");

    m.def("approx_equal",
          [](double a, double b, double rtol, double atol) {
              return yatrices::approx_equal(a, b, rtol, atol);
          },
          py::arg("a"), py::arg("b"), py::arg("rtol") = yatrices::default_tolerance<double>(),
          py::arg("atol") = yatrices::default_tolerance<double>(),
          "True when two numbers are approximately equal (never use == on floats).");
}
