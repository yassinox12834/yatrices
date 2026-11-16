// after/cpp/memory.hpp
//
// STUB. Comments only: nothing in this file is compiled, and no code is written for you.
// Subject: buffer pool for aligned allocations.
//
// When you start it: move this file to cpp/include/yatrices/memory.hpp, add
//     #pragma once
//     namespace yatrices { ... }
// and put the definitions in cpp/src/memory.cpp (add that file to cpp/CMakeLists.txt).
//
// Item numbers refer to why_I_do_this.pdf, version 4. The items are listed in the
// order to write them (Part B of the PDF). A signature is a suggestion for the
// interface: rename freely, and every body is yours to write.
//
// 1 item(s).
//
// [ ] Item 359: Buffer pool
//       difficulty 5/10, stage 11, needs first: 205
//       place: new class or type
//       suggested:
//           class BufferPool { public: void* acquire(std::size_t bytes)
//           void release(void* p, std::size_t bytes) noexcept
//           }
//
