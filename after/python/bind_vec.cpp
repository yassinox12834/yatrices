// after/python/bind_vec.cpp
//
// STUB. Comments only: no code is written for you.
// Kind: pybind11 binding code. When you start it: move this file to python/bindings/.
//
// Item numbers refer to why_I_do_this.pdf, version 4, listed in the order to write them.
// A signature is a suggestion for the interface; every body is yours to write.
//
// [ ] Item 70: Python bindings
//       difficulty 6/10, stage 0, needs first: 1, 102
//       place: pybind11 binding
//       suggested:
//           PYBIND11_MODULE(_core, m) { py::class_<Vec<double>>(m, "Vec").def(py::init<std::size_t>()).def("dot", &Vec<double>::dot)
//           }
//
// [ ] Item 387: Python operator protocol
//       difficulty 3/10, stage 13, needs first: 70
//       place: pybind11 binding
//       suggested:
//           .def(py::self + py::self).def(py::self += py::self).def(double() * py::self).def("__len__", &Vec<double>::size)
//
