// after/cpp/geometry.hpp
//
// STUB. Comments only: nothing in this file is compiled, and no code is written for you.
// Subject: rotations, projections, transformations, distances.
//
// When you start it: move this file to cpp/include/yatrices/geometry.hpp, add
//     #pragma once
//     namespace yatrices { ... }
// and put the definitions in cpp/src/geometry.cpp (add that file to cpp/CMakeLists.txt).
//
// Item numbers refer to why_I_do_this.pdf, version 4. The items are listed in the
// order to write them (Part B of the PDF). A signature is a suggestion for the
// interface: rename freely, and every body is yours to write.
//
// 13 item(s).
//
// [ ] Item 221: Scaling, shear and reflection matrices
//       difficulty 1/10, stage 2, needs first: 7
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> scaling_2d(T sx, T sy)
//           Matrix<T> shear_2d(T k)
//           Matrix<T> reflection_2d(T theta)
//
// [ ] Item 225: Distance from a point to a line or plane
//       difficulty 2/10, stage 2, needs first: 173
//       place: free function in namespace yatrices
//       suggested:
//           T distance_to_line(const Vec<T>& p, const Vec<T>& a, const Vec<T>& direction)
//           T distance_to_plane(const Vec<T>& p, const Vec<T>& a, const Vec<T>& normal)
//
// [ ] Item 226: Area, volume and orientation tests
//       difficulty 2/10, stage 2, needs first: 11
//       place: free function in namespace yatrices
//       suggested:
//           T orient2d(const Vec<T>& a, const Vec<T>& b, const Vec<T>& c)
//           T triangle_area(const Vec<T>& a, const Vec<T>& b, const Vec<T>& c)
//
// [ ] Item 111: Rotation matrices
//       difficulty 3/10, stage 2, needs first: 7, 14
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> rotation_2d(T theta)
//           Matrix<T> rotation_3d(const Vec<T>& axis, T theta)
//
// [ ] Item 112: Homogeneous coordinates
//       difficulty 3/10, stage 2, needs first: 7
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> translation(const Vec<T>& t)
//           Matrix<T> rigid_transform(const Matrix<T>& R, const Vec<T>& t)
//
// [ ] Item 222: Euler angles
//       difficulty 4/10, stage 2, needs first: 111
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> from_euler_zyx(T yaw, T pitch, T roll)
//           Vec<T> to_euler_zyx(const Matrix<T>& R)
//
// [ ] Item 223: Axis and angle of a rotation
//       difficulty 4/10, stage 2, needs first: 111, 9
//       place: result struct plus free function
//       suggested:
//           template <typename T> struct AxisAngle { Vec<T> axis
//           T angle
//           }
//           AxisAngle<T> to_axis_angle(const Matrix<T>& R)
//
// [ ] Item 224: Look-at and perspective matrices
//       difficulty 4/10, stage 2, needs first: 112
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> look_at(const Vec<T>& eye, const Vec<T>& target, const Vec<T>& up)
//           Matrix<T> perspective(T fov_y, T aspect, T near, T far)
//
// [ ] Item 248: Barycentric coordinates
//       difficulty 3/10, stage 3, needs first: 79
//       place: free function in namespace yatrices
//       suggested:
//           Vec<T> barycentric(const Vec<T>& p, const Vec<T>& a, const Vec<T>& b, const Vec<T>& c)
//
// [ ] Item 308: Principal axes of a quadratic form
//       difficulty 3/10, stage 7, needs first: 48, 191
//       place: result struct plus free function
//       suggested:
//           template <typename T> struct Ellipse { Matrix<T> axes
//           Vec<T> half_lengths
//           }
//           Ellipse<T> principal_axes(const Matrix<T>& A)
//
// [ ] Item 321: Best-fit line and plane
//       difficulty 5/10, stage 8, needs first: 55
//       place: result struct plus free function
//       suggested:
//           template <typename T> struct Plane { Vec<T> point, normal
//           }
//           Plane<T> fit_plane(const Matrix<T>& points)
//
// [ ] Item 320: Kabsch algorithm
//       difficulty 5/10, stage 8, needs first: 92
//       place: free function in namespace yatrices
//       suggested:
//           Matrix<T> kabsch(const Matrix<T>& P, const Matrix<T>& Q)
//
// [ ] Item 378: Quaternions
//       difficulty 5/10, stage 12, needs first: 111, 222
//       place: new class or type
//       suggested:
//           template <typename T> struct Quaternion { T w, x, y, z
//           }
//           Quaternion<T> operator*(Quaternion<T> a, Quaternion<T> b)
//           Matrix<T> to_matrix(Quaternion<T> q)
//
