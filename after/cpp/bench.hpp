// after/cpp/bench.hpp
//
// STUB. Comments only: nothing in this file is compiled, and no code is written for you.
// Subject: timing tools and benchmark programs.
//
// When you start it: move this file to cpp/include/yatrices/bench.hpp, add
//     #pragma once
//     namespace yatrices { ... }
// and put the definitions in cpp/src/bench.cpp (add that file to cpp/CMakeLists.txt).
//
// Item numbers refer to why_I_do_this.pdf, version 4. The items are listed in the
// order to write them (Part B of the PDF). A signature is a suggestion for the
// interface: rename freely, and every body is yours to write.
//
// 4 item(s).
//
// [ ] Item 105: Benchmark harness
//       difficulty 2/10, stage 2, needs first: 7
//       place: free function in namespace yatrices
//       suggested:
//           template <typename F> double median_seconds(F f, int warmup, int repeats)
//
// [ ] Item 106: Loop order in matmul
//       difficulty 2/10, stage 2, needs first: 7
//       place: example or benchmark program
//       suggested:
//           PROGRAM: matmul_ijk, matmul_ikj, matmul_kij timed for n = 256, 512, 1024
//
// [ ] Item 229: Measured complexity
//       difficulty 2/10, stage 2, needs first: 105
//       place: free function in namespace yatrices
//       suggested:
//           double fitted_exponent(const std::vector<std::size_t>& n, const std::vector<double>& seconds)
//
// [ ] Item 228: Seeing the cache
//       difficulty 3/10, stage 2, needs first: 105
//       place: example or benchmark program
//       suggested:
//           PROGRAM: bench_cache_steps, prints n and nanoseconds per element as CSV
//
