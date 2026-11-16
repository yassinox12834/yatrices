// after/cpp/kernels.hpp
//
// STUB. Comments only: nothing in this file is compiled, and no code is written for you.
// Subject: low-level fast loops (cache blocking, SIMD); internal, not public API.
//
// When you start it: move this file to cpp/include/yatrices/kernels.hpp, add
//     #pragma once
//     namespace yatrices { ... }
// and put the definitions in cpp/src/kernels.cpp (add that file to cpp/CMakeLists.txt).
//
// Item numbers refer to why_I_do_this.pdf, version 4. The items are listed in the
// order to write them (Part B of the PDF). A signature is a suggestion for the
// interface: rename freely, and every body is yours to write.
//
// 8 item(s).
//
// [ ] Item 348: Aliasing and restrict
//       difficulty 4/10, stage 11, needs first: 227
//       place: internal kernel, not in the public API
//       suggested:
//           void axpy_kernel(std::size_t n, T alpha, const T* __restrict x, T* __restrict y) noexcept
//
// [ ] Item 360: Optional BLAS and LAPACK backend
//       difficulty 4/10, stage 11, needs first: 202
//       place: internal kernel, not in the public API
//       suggested:
//           option(YATRICES_USE_BLAS ...)
//           #ifdef YATRICES_USE_BLAS cblas_dgemm(CblasRowMajor, ...) #endif
//
// [ ] Item 350: Dot and axpy with AVX2 and FMA by hand
//       difficulty 6/10, stage 11, needs first: 183, 349
//       place: internal kernel, not in the public API
//       suggested:
//           double dot_avx2(const double* x, const double* y, std::size_t n) noexcept
//           void axpy_avx2(std::size_t n, double alpha, const double* x, double* y) noexcept
//
// [ ] Item 351: Runtime CPU dispatch
//       difficulty 6/10, stage 11, needs first: 350
//       place: internal kernel, not in the public API
//       suggested:
//           using DotFn = double (*)(const double*, const double*, std::size_t)
//           DotFn select_dot() noexcept
//
// [ ] Item 63: Cache blocking for matmul
//       difficulty 7/10, stage 11, needs first: 7
//       place: internal kernel, not in the public API
//       suggested:
//           void matmul_blocked(const T* a, const T* b, T* c, std::size_t m, std::size_t n, std::size_t p, std::size_t block)
//
// [ ] Item 352: Cache-blocked transpose
//       difficulty 5/10, stage 11, needs first: 3, 63
//       place: internal kernel, not in the public API
//       suggested:
//           void transpose_blocked(const T* a, T* out, std::size_t m, std::size_t n, std::size_t block)
//
// [ ] Item 64: SIMD and threads
//       difficulty 8/10, stage 11, needs first: 63
//       place: internal kernel, not in the public API
//       suggested:
//           void matmul_avx2(const double* a, const double* b, double* c, std::size_t m, std::size_t n, std::size_t p)
//           #pragma omp parallel for on the outer block loop
//
// [ ] Item 399: Packed matmul with a micro-kernel
//       difficulty 9/10, stage 14, needs first: 63, 350
//       place: internal kernel, not in the public API
//       suggested:
//           void pack_a(...)
//           void pack_b(...)
//           void micro_kernel_4x8(std::size_t k, const double* a, const double* b, double* c, std::size_t ldc) noexcept
//
