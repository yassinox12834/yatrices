// after/python/bind_errors.cpp
//
// STUB. Comments only: no code is written for you.
// Kind: pybind11 binding code. When you start it: move this file to python/bindings/.
//
// Item numbers refer to why_I_do_this.pdf, version 4, listed in the order to write them.
// A signature is a suggestion for the interface; every body is yours to write.
//
// [ ] Item 388: Mapping errors to Python exceptions
//       difficulty 3/10, stage 13, needs first: 70, 104
//       place: pybind11 binding
//       suggested:
//           py::register_exception<yatrices::SizeMismatchError>(m, "SizeMismatchError", PyExc_ValueError)
//
