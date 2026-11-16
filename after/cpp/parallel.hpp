// after/cpp/parallel.hpp
//
// STUB. Comments only: nothing in this file is compiled, and no code is written for you.
// Subject: thread pool and parallel loops.
//
// When you start it: move this file to cpp/include/yatrices/parallel.hpp, add
//     #pragma once
//     namespace yatrices { ... }
// and put the definitions in cpp/src/parallel.cpp (add that file to cpp/CMakeLists.txt).
//
// Item numbers refer to why_I_do_this.pdf, version 4. The items are listed in the
// order to write them (Part B of the PDF). A signature is a suggestion for the
// interface: rename freely, and every body is yours to write.
//
// 1 item(s).
//
// [ ] Item 356: Thread pool and parallel-for
//       difficulty 6/10, stage 11, needs first: 63
//       place: new class or type
//       suggested:
//           class ThreadPool { public: explicit ThreadPool(unsigned n)
//           template <typename F> void parallel_for(std::size_t begin, std::size_t end, F body)
//           }
//
