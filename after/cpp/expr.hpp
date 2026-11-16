// after/cpp/expr.hpp
//
// STUB. Comments only: nothing in this file is compiled, and no code is written for you.
// Subject: expression templates (lazy evaluation of a + b + c).
//
// When you start it: move this file to cpp/include/yatrices/expr.hpp, add
//     #pragma once
//     namespace yatrices { ... }
// and put the definitions in cpp/src/expr.cpp (add that file to cpp/CMakeLists.txt).
//
// Item numbers refer to why_I_do_this.pdf, version 4. The items are listed in the
// order to write them (Part B of the PDF). A signature is a suggestion for the
// interface: rename freely, and every body is yours to write.
//
// 1 item(s).
//
// [ ] Item 358: Expression templates
//       difficulty 8/10, stage 11, needs first: 103
//       place: new class or type
//       suggested:
//           template <typename L, typename R> struct AddExpr { const L& l
//           const R& r
//           auto operator[](std::size_t i) const { return l[i] + r[i]
//           } }
//
