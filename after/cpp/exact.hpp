// after/cpp/exact.hpp
//
// STUB. Comments only: nothing in this file is compiled, and no code is written for you.
// Subject: exact arithmetic: integers, fractions, arithmetic modulo p.
//
// When you start it: move this file to cpp/include/yatrices/exact.hpp, add
//     #pragma once
//     namespace yatrices { ... }
// and put the definitions in cpp/src/exact.cpp (add that file to cpp/CMakeLists.txt).
//
// Item numbers refer to why_I_do_this.pdf, version 4. The items are listed in the
// order to write them (Part B of the PDF). A signature is a suggestion for the
// interface: rename freely, and every body is yours to write.
//
// 4 item(s).
//
// [ ] Item 246: Elimination modulo a prime
//       difficulty 4/10, stage 3, needs first: 22
//       place: free function in namespace yatrices
//       suggested:
//           std::size_t rank_mod_p(std::vector<std::vector<long long>> a, long long p)
//           long long det_mod_p(std::vector<std::vector<long long>> a, long long p)
//
// [ ] Item 247: Elimination over GF(2) with bitsets
//       difficulty 4/10, stage 3, needs first: 246
//       place: free function in namespace yatrices
//       suggested:
//           std::size_t rank_gf2(std::vector<std::bitset<64>> rows)
//
// [ ] Item 126: Bareiss elimination
//       difficulty 5/10, stage 3, needs first: 22
//       place: free function in namespace yatrices
//       suggested:
//           long long det_bareiss(std::vector<long long> a, std::size_t n)
//
// [ ] Item 245: Exact rational solve
//       difficulty 5/10, stage 3, needs first: 126
//       place: new class or type
//       suggested:
//           struct Fraction { long long num, den
//           }
//           std::vector<Fraction> solve_exact(std::vector<std::vector<Fraction>> a, std::vector<Fraction> b)
//
