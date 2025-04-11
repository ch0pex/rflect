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

#include "functions.hpp"

namespace sim::math {

// clang-format off

template<is_scalar T>
struct Vec2 {
  constexpr Vec2() : x {}, y {} { }

  constexpr Vec2(T const x, T y) : x {x}, y {y} { }

  constexpr explicit Vec2(T const s) : x {s}, y {s} { }

  constexpr Vec2 operator-() { return {-x, -y}; }

  constexpr Vec2 operator+() { return {+x, +y}; }

  constexpr Vec2 operator+=(Vec2 const v2) { x += v2.x, y += v2.y; return *this; }

  constexpr Vec2 operator-=(Vec2 const v2) { x -= v2.x, y -= v2.y; return *this; }

  constexpr Vec2 operator*=(Vec2 const v2) { x *= v2.x, y *= v2.y; return *this; }

  constexpr Vec2 operator/=(Vec2 const v2) { x /= v2.x, y /= v2.y; return *this; }

  constexpr Vec2 operator+=(T const s) { *this += Vec2(s); return *this; }

  constexpr Vec2 operator-=(T const s) { *this -= Vec2(s); return *this; }

  constexpr Vec2 operator*=(T const s) { *this *= Vec2(s); return *this; }

  constexpr Vec2 operator/=(T const s) { *this /= Vec2(s); return *this; }

  template<is_scalar U> explicit constexpr operator Vec2<U>() const { return {x, y}; }

  T x, y;
};

template<is_scalar T>
struct Vec3 : ScalarOperators {
  constexpr Vec3() : x {}, y {}, z {} { }

  constexpr Vec3(Vec2<T> const vec2, T z) : x {vec2.x}, y {vec2.y}, z {z} { }

  constexpr Vec3(T const x, T const y, T const z) : x {x}, y {y}, z {z} { }

  constexpr explicit Vec3(T const s) : x {s}, y {s}, z {s} { }

  constexpr explicit Vec3(Vec4<T> const vec) : x {vec.x}, y {vec.y}, z {vec.z} { }

  constexpr auto xy() const -> Vec2<T> { return {x, y}; }

  constexpr Vec3 operator-() const { return {-x, -y, -z}; }

  constexpr Vec3 operator+() const { return {+x, +y, +z}; }

  constexpr Vec3 operator+=(Vec3 const v2) { x += v2.x, y += v2.y, z += v2.z; return *this; }

  constexpr Vec3 operator-=(Vec3 const v2) { x -= v2.x, y -= v2.y, z -= v2.z; return *this; }

  constexpr Vec3 operator*=(Vec3 const v2) { x *= v2.x, y *= v2.y, z *= v2.z; return *this; }

  constexpr Vec3 operator/=(Vec3 const v2) { x /= v2.x, y /= v2.y, z /= v2.z; return *this; }

  constexpr Vec3 operator+=(T const s) { *this += Vec3(s); return *this; }

  constexpr Vec3 operator-=(T const s) { *this -= Vec3(s); return *this; }

  constexpr Vec3 operator*=(T const s) { *this *= Vec3(s); return *this; }

  constexpr Vec3 operator/=(T const s) { *this /= Vec3(s); return *this; }

  template<is_scalar U> explicit constexpr operator Vec3<U>() const { return {x, y, z}; }

  T x, y, z;
};

template<is_scalar T>
struct Vec4 : ScalarOperators  {
  constexpr Vec4() : x {}, y {}, z {}, w {} { }

  constexpr Vec4(Vec2<T> const xy, T const z, T const w) : x {xy.x}, y {xy.y}, z {z}, w {w} { }

  constexpr Vec4(Vec3<T> const xyz, T const w) : x {xyz.x}, y {xyz.y}, z {xyz.z}, w {w} { }

  constexpr Vec4(T const x, T const y, T const  z, T const w) : x {x}, y {y}, z {z}, w {w} { }

  constexpr explicit Vec4(Vec3<T> const xyz) : x {xyz.x}, y {xyz.y}, z {xyz.z}, w {} { }

  constexpr explicit Vec4(T const s) : x {s}, y {s}, z {s}, w {s} { }

  constexpr Vec3<T> xyz() const { return {x, y, z, w}; }

  constexpr Vec4 operator-() const { return {-x, -y, -z, -w}; }

  constexpr Vec4 operator+() const { return {+x, +y, +z, +w}; }

  constexpr Vec4 operator+=(Vec4 const v2) { x += v2.x, y += v2.y, z += v2.z, w += v2.w; return *this; }

  constexpr Vec4 operator-=(Vec4 const v2) { x -= v2.x, y -= v2.y, z -= v2.z, w -= v2.w; return *this; }

  constexpr Vec4 operator*=(Vec4 const v2) { x *= v2.x, y *= v2.y, z *= v2.z, w *= v2.w; return *this; }

  constexpr Vec4 operator/=(Vec4 const v2) { x /= v2.x, y /= v2.y, z /= v2.z,  w /= v2.w; return *this; }

  constexpr Vec4 operator+=(T const s) { *this += Vec4(s); return *this; }

  constexpr Vec4 operator-=(T const s) { *this -= Vec4(s); return *this; }

  constexpr Vec4 operator*=(T const s) { *this *= Vec4(s); return *this; }

  constexpr Vec4 operator/=(T const s) { *this /= Vec4(s); return *this; }

  template<is_scalar U> explicit constexpr operator Vec4<U>() const { return {x, y, z, w}; }

  T x, y, z, w;
};

// clang-format on

using vec2 = Vec2<scalar>;
using vec3 = Vec3<scalar>;
using vec4 = Vec4<scalar>;

static_assert(vector<vec2>);
static_assert(vector<vec3>);
static_assert(vector<vec4>);

} // namespace sim::math
