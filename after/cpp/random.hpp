// after/cpp/random.hpp
//
// STUB. Comments only: nothing in this file is compiled, and no code is written for you.
// Subject: reproducible random numbers and random test matrices.
//
// When you start it: move this file to cpp/include/yatrices/random.hpp, add
//     #pragma once
//     namespace yatrices { ... }
// and put the definitions in cpp/src/random.cpp (add that file to cpp/CMakeLists.txt).
//
// Item numbers refer to why_I_do_this.pdf, version 4. The items are listed in the
// order to write them (Part B of the PDF). A signature is a suggestion for the
// interface: rename freely, and every body is yours to write.
//
// 2 item(s).
//
// [ ] Item 197: Reproducible random generator
//       difficulty 3/10, stage 1, needs first: 1
//       place: new class or type
//       suggested:
//           class Rng { std::uint64_t s[4]
//           public: explicit Rng(std::uint64_t seed)
//           std::uint64_t next() noexcept
//           double uniform() noexcept
//           double normal() noexcept
//           }
//
// [ ] Item 116: Random test matrices
//       difficulty 4/10, stage 5, needs first: 8, 38
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> random_spd<T>(n, Rng& rng)
//           random_orthogonal<T>(n, rng)
//           random_with_cond<T>(n, T kappa, rng)
//
