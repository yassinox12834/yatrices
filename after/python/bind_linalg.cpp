// after/python/bind_linalg.cpp
//
// STUB. Comments only: no code is written for you.
// Kind: pybind11 binding code. When you start it: move this file to python/bindings/.
//
// Item numbers refer to why_I_do_this.pdf, version 4, listed in the order to write them.
// A signature is a suggestion for the interface; every body is yours to write.
//
// [ ] Item 391: Releasing the GIL
//       difficulty 4/10, stage 13, needs first: 386
//       place: pybind11 binding
//       suggested:
//           { py::gil_scoped_release release
//           result = lu_factor(std::move(A))
//           }
//
// [ ] Item 393: Batched small solves
//       difficulty 5/10, stage 13, needs first: 357, 390
//       place: pybind11 binding
//       suggested:
//           py::array_t<double> solve_batch_3x3(py::array_t<double> A, py::array_t<double> b)
//
