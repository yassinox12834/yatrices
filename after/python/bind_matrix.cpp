// after/python/bind_matrix.cpp
//
// STUB. Comments only: no code is written for you.
// Kind: pybind11 binding code. When you start it: move this file to python/bindings/.
//
// Item numbers refer to why_I_do_this.pdf, version 4, listed in the order to write them.
// A signature is a suggestion for the interface; every body is yours to write.
//
// [ ] Item 386: Binding the Matrix class
//       difficulty 4/10, stage 13, needs first: 70
//       place: pybind11 binding
//       suggested:
//           py::class_<Matrix<double>>(m, "Matrix").def_property_readonly("shape", ...).def("__getitem__", ...).def("__matmul__", ...)
//
// [ ] Item 389: Buffer protocol
//       difficulty 6/10, stage 13, needs first: 386, 102
//       place: pybind11 binding
//       suggested:
//           py::class_<Matrix<double>>(m, "Matrix", py::buffer_protocol()).def_buffer([](Matrix<double>& a) -> py::buffer_info { ... })
//
// [ ] Item 390: Accepting NumPy arrays without a copy
//       difficulty 6/10, stage 13, needs first: 389
//       place: pybind11 binding
//       suggested:
//           double dot_numpy(py::array_t<double, py::array::c_style> x, py::array_t<double, py::array::c_style> y)
//
