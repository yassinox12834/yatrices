// after/cpp/fixed.hpp
//
// STUB. Comments only: nothing in this file is compiled, and no code is written for you.
// Subject: small matrices whose size is known at compile time.
//
// When you start it: move this file to cpp/include/yatrices/fixed.hpp, add
//     #pragma once
//     namespace yatrices { ... }
// and put the definitions in cpp/src/fixed.cpp (add that file to cpp/CMakeLists.txt).
//
// Item numbers refer to why_I_do_this.pdf, version 4. The items are listed in the
// order to write them (Part B of the PDF). A signature is a suggestion for the
// interface: rename freely, and every body is yours to write.
//
// 1 item(s).
//
// [ ] Item 357: Fixed-size small matrices
//       difficulty 4/10, stage 11, needs first: 69
//       place: new class or type
//       suggested:
//           template <typename T, std::size_t R, std::size_t C> class SmallMatrix { std::array<T, R * C> a
//           }
//           using Mat3d = SmallMatrix<double, 3, 3>
//
