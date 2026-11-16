// after/cpp/factories.hpp
//
// STUB. Comments only: nothing in this file is compiled, and no code is written for you.
// Subject: functions that build standard matrices (identity, zeros, random).
//
// When you start it: move this file to cpp/include/yatrices/factories.hpp, add
//     #pragma once
//     namespace yatrices { ... }
// and put the definitions in cpp/src/factories.cpp (add that file to cpp/CMakeLists.txt).
//
// Item numbers refer to why_I_do_this.pdf, version 4. The items are listed in the
// order to write them (Part B of the PDF). A signature is a suggestion for the
// interface: rename freely, and every body is yours to write.
//
// 1 item(s).
//
// [ ] Item 8: Identity, zeros, diagonal, random
//       difficulty 1/10, stage 1, needs first: 1, 197
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> identity<T>(std::size_t n)
//           zeros<T>(m, n)
//           diag(const Vec<T>& d)
//           random_uniform<T>(m, n, Rng& rng)
//
