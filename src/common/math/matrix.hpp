/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file utils.hpp
 * @version 1.0
 * @date 03/02/2025
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "vector.hpp"

namespace sim::math {

// clang-format off

template<is_scalar T>
struct Mat3x3 : ScalarOperators {
  constexpr Mat3x3() : x {}, y {}, z {} { }

  constexpr Mat3x3(Vec3<T> const x, Vec3<T> const y, Vec3<T> const z) : x {x}, y {y}, z {z} { }

  constexpr explicit Mat3x3(Mat4x4<T> const& m) : x {Vec3<T>(m.x)}, y {Vec3<T>(m.y)}, z {Vec3<T>(m.z)} { }

  constexpr explicit Mat3x3(is_scalar auto s) : x {s}, y {s}, z {s} { }

  constexpr Mat3x3 operator-() const { return {-x, -y, -z}; }

  constexpr Mat3x3 operator+() const { return {+x, +y, +z}; }

  constexpr Mat3x3 operator+=(Mat3x3 const& m2) { x += m2.x, y += m2.y, z += m2.z; return *this; }

  constexpr Mat3x3 operator-=(Mat3x3 const& m2) { x -= m2.x, y -= m2.y, z -= m2.z; return *this; }

  constexpr Mat3x3 operator*=(Mat3x3 const m2) {
    auto const m2_trans = transpose(m2);
    x = { dot(x, m2_trans.x), dot(x, m2_trans.y), dot(x, m2_trans.z)};
    y = { dot(y, m2_trans.x), dot(y, m2_trans.y), dot(y, m2_trans.z)};
    z = { dot(z, m2_trans.x), dot(z, m2_trans.y), dot(z, m2_trans.z)};
    return *this;
  }

  constexpr Mat3x3 operator/=(Mat3x3 const m2) {
    *this *= inverse(m2);
    return *this;
  }

  constexpr Mat3x3 operator+=(is_scalar auto const s) { *this += Mat3x3{s}; return *this; }

  constexpr Mat3x3 operator-=(is_scalar auto const s) { *this -= Mat3x3{s}; return *this; }

  constexpr Mat3x3 operator*=(is_scalar auto const s) { x *= s, y *= s, z *= s; return *this; }

  constexpr Mat3x3 operator/=(is_scalar auto const s) { x /= s, y /= s, z /= s; return *this; }

  // constexpr Vec3<T> operator/=(Vec3<T> const s) { x /= s, y /= s, z /= s; return *this; }

  Vec3<T> x, y, z;
};

template<is_scalar T>
struct Mat4x4 : ScalarOperators {
  constexpr Mat4x4() : x {}, y {}, z {}, w {} { }

  constexpr Mat4x4(Vec4<T> const x, Vec4<T> const y, Vec4<T> const z, Vec4<T> const w) : x {x}, y {y}, z {z}, w {w} { }

  constexpr Mat4x4(Mat3x3<T> const& xyz, T const w) : x {xyz.x}, y {xyz.y}, z {xyz.z}, w {w} { }

  constexpr explicit Mat4x4(is_scalar auto s) : x {s}, y {s}, z {s}, w {s} { }

  constexpr explicit Mat4x4(Mat3x3<T> const& xyz) : x {xyz.x}, y {xyz.y}, z {xyz.z}, w {} { }

  constexpr Mat4x4 operator-() const { return {-x, -y, -z, -w}; }

  constexpr Mat4x4 operator+() const { return {+x, +y, +z, +w}; }

  constexpr Mat4x4 operator+=(Mat4x4 const& m2) { x += m2.x, y += m2.y, z += m2.z, w += m2.w; return *this; }

  constexpr Mat4x4 operator-=(Mat4x4 const& m2) { x -= m2.x, y -= m2.y, z -= m2.z, w -= m2.w; return *this; }

  constexpr Mat4x4 operator*=(Mat4x4 const& m2) {
    auto const m2_trans = transpose(m2);
    x = {dot(x, m2_trans.x), dot(x, m2_trans.y), dot(x, m2_trans.z), dot(x, m2_trans.w)};
    y = {dot(y, m2_trans.x), dot(y, m2_trans.y), dot(y, m2_trans.z), dot(y, m2_trans.w)};
    z = {dot(z, m2_trans.x), dot(z, m2_trans.y), dot(z, m2_trans.z), dot(z, m2_trans.w)};
    w = {dot(w, m2_trans.x), dot(w, m2_trans.y), dot(w, m2_trans.z), dot(w, m2_trans.w)};
    return *this;
  }

  constexpr Mat4x4 operator/=(Mat4x4 const& m2) {
   *this *= inverse(m2);
    return *this;
  }

  constexpr Mat4x4 operator+=(is_scalar auto const s) { *this += Mat4x4{s}; return *this; }

  constexpr Mat4x4 operator-=(is_scalar auto const s) { *this -= Mat4x4{s}; return *this; }

  constexpr Mat4x4 operator*=(is_scalar auto const s) { x *= s, y *= s, z *= s, w *= s; return *this; }

  constexpr Mat4x4 operator/=(is_scalar auto const s) { x /= s, y /= s, z /= s, w /= s; return *this; }

  Vec4<T> x, y, z, w;
};

// clang-format on

using mat3 = Mat3x3<scalar>;
using mat4 = Mat4x4<scalar>;

static_assert(matrix<mat3>);
static_assert(matrix<mat4>);

} // namespace reveal3d::math
