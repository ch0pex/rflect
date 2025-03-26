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

#include "concepts.hpp"
#include "vector.hpp"

#include <iostream>

namespace sim::math {

// clang-format off

template<is_scalar T>
std::ostream& operator<<(std::ostream& os, Vec2<T> const obj) {
  os << "{" << obj.x << ", " << obj.y << "}";
  return os;
}

template<is_scalar T>
std::ostream& operator<<(std::ostream& os, Vec3<T> const obj) {
  os << "{" << obj.x << ", " << obj.y << ", " << obj.z << "}";
  return os;
}

template<is_scalar T>
std::ostream& operator<<(std::ostream& os, Vec4<T> const obj) {
  os << "{" << obj.x << ", " << obj.y << ", " << obj.z << ", " << obj.w << "}";
  return os;
}

template<is_scalar T>
std::ostream& operator<<(std::ostream& os, Mat3x3<T> const& mat) {
  os << mat.x  << "\n" << mat.y << "\n" << mat.z << "\n";
  return os;
}

template<is_scalar T>
std::ostream& operator<<(std::ostream& os, Mat4x4<T> const& mat) {
  os << mat.x  << "\n" << mat.y << "\n" << mat.z << "\n" << mat.w << "\n";
  return os;
}

template<is_scalar T> constexpr bool operator==(Vec2<T> const lhs, Vec2<T> const rhs) { return lhs.x == rhs.x and lhs.y == rhs.y; }

template<is_scalar T> constexpr bool operator==(Vec3<T> const lhs, Vec3<T> const rhs) { return lhs.x == rhs.x and lhs.y == rhs.y and lhs.z == rhs.z; }

template<is_scalar T> constexpr bool operator==(Vec4<T> const lhs, Vec4<T> const rhs) { return lhs.x == rhs.x and lhs.y == rhs.y and lhs.z == rhs.z and lhs.w == rhs.w; }

template<is_scalar T> constexpr bool operator==(Mat3x3<T> const& lhs, Mat3x3<T> const& rhs) { return lhs.x == rhs.x and lhs.y == rhs.y and lhs.z == rhs.z; }

template<is_scalar T> constexpr bool operator==(Mat4x4<T> const& lhs, Mat4x4<T> const& rhs) { return lhs.x == rhs.x and lhs.y == rhs.y and lhs.z == rhs.z and lhs.w == rhs.w; }

template<algebra_type T> constexpr  bool operator!=(T const& lhs, T const& rhs) { return !(lhs == rhs); }

template<algebra_type T> constexpr auto operator+(T lh, T const rh) { lh += rh; return lh; }

template<algebra_type T> constexpr auto operator-(T lh, T const rh) { lh -= rh; return lh; }

template<algebra_type T> constexpr auto operator*(T lh, T const rh) { lh *= rh; return lh; }

template<algebra_type T> constexpr auto operator/(T lh, T const rh) { lh /= rh; return lh; }

constexpr auto operator+(algebra_type auto lh, is_scalar auto const rh) { lh += rh; return lh; }

constexpr auto operator-(algebra_type auto lh, is_scalar auto const rh) { lh -= rh; return lh; }

constexpr auto operator*(algebra_type auto lh, is_scalar auto const rh) { lh *= rh; return lh; }

constexpr auto operator/(algebra_type auto lh, is_scalar auto const rh) { lh /= rh; return lh; }

constexpr auto operator+(is_scalar auto const lh, algebra_type auto rh) { rh += lh; return rh; }

constexpr auto operator-(is_scalar auto const lh, algebra_type auto rh) { rh -= lh; return rh; }

constexpr auto operator*(is_scalar auto const lh, algebra_type auto rh) { rh *= lh; return rh; }

constexpr auto operator/(is_scalar auto const lh, algebra_type auto rh) { rh /= lh; return rh; }

template<is_scalar T>
constexpr auto operator*(Mat3x3<T> const& mat3, Vec3<T> const vec3) -> Vec3<T> {
   return Vec3<T>{dot(mat3.x, vec3), dot(mat3.y , vec3), dot(mat3.z , vec3)};
}

// template<is_scalar T>
// constexpr auto operator/(Mat3x3<T> const& mat3, Vec3<T> const vec3) -> Vec3<T> {
   // return Vec3<T>{dot(mat3.x, vec3), dot(mat3.y, vec3), dot(mat3.z, vec3)};
// }

template<is_scalar T>
constexpr auto operator*(Mat4x4<T> const& mat4, Vec4<T> const vec4) -> Vec4<T> {
   return Vec4<T>{dot(mat4.x, vec4), dot(mat4.y, vec4), dot(mat4.z, vec4), dot(mat4.w, vec4)};
}

// template<scalar T>
// constexpr auto operator/(Mat4x4<T> const& mat4, Vec4<T> const vec4) -> Vec4<T> {
   // return Vec4<T>{mat4.x / vec4, mat4.y / vec4, mat4.z / vec4, mat4.w / vec4};
// }

// clang-format on

} // namespace sim::math
