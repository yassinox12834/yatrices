// after/cpp/io.hpp
//
// STUB. Comments only: nothing in this file is compiled, and no code is written for you.
// Subject: reading and writing files (CSV, .npy, Matrix Market).
//
// When you start it: move this file to cpp/include/yatrices/io.hpp, add
//     #pragma once
//     namespace yatrices { ... }
// and put the definitions in cpp/src/io.cpp (add that file to cpp/CMakeLists.txt).
//
// Item numbers refer to why_I_do_this.pdf, version 4. The items are listed in the
// order to write them (Part B of the PDF). A signature is a suggestion for the
// interface: rename freely, and every body is yours to write.
//
// 3 item(s).
//
// [ ] Item 260: CSV reader
//       difficulty 2/10, stage 4, needs first: 1
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> read_csv<T>(const std::string& path, bool has_header)
//
// [ ] Item 369: Matrix Market reader
//       difficulty 3/10, stage 12, needs first: 362
//       place: free function in namespace yatrices
//       suggested:
//           CooMatrix<T> read_matrix_market<T>(const std::string& path)
//
// [ ] Item 396: Saving and loading .npy files
//       difficulty 4/10, stage 13, needs first: 102
//       place: free function in namespace yatrices
//       suggested:
//           void save_npy(const std::string& path, const Matrix<double>& A)
//           Matrix<double> load_npy(const std::string& path)
//
