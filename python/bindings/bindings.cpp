#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <cstddef>
#include <string>
#include <vector>

#include "yatrices/errors.hpp"
#include "yatrices/vector.hpp"

namespace py = pybind11;
using yatrices::Vec;

namespace {

PyObject* E_base = nullptr;
PyObject* E_index = nullptr;
PyObject* E_mismatch = nullptr;
PyObject* E_empty = nullptr;
PyObject* E_wrong_size = nullptr;
PyObject* E_zero = nullptr;
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

}

PYBIND11_MODULE(_yatricespy, m) {
    m.doc() = "yatricespy: Python front end of YatricesEngine";

    bind_errors(m);
    bind_vec<double>(m, "Vec", "float64");
    bind_vec<float>(m, "Vecf", "float32");

    m.def("approx_equal",
          [](double a, double b, double rtol, double atol) {
              return yatrices::approx_equal(a, b, rtol, atol);
          },
          py::arg("a"), py::arg("b"), py::arg("rtol") = yatrices::default_tolerance<double>(),
          py::arg("atol") = yatrices::default_tolerance<double>(),
          "True when two numbers are approximately equal (never use == on floats).");
}
