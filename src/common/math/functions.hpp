/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file functions.hpp
 * @version 1.0
 * @date 01/03/2024
 * @brief Math functions
 *
 * Math inline template functions for vector, scalar and matrix
 */
#pragma once

#include "utils/primitive_types.hpp"
#include "concepts.hpp"

#include <cmath>
#include <numbers>


namespace sim::math {

// clang-format off

template<is_scalar T>
constexpr T determinant(Mat3x3<T> const& m) {
  return (m.x.x * ((m.y.y * m.z.z) - (m.y.z * m.z.y))) -
         (m.x.y * ((m.y.x * m.z.z) - (m.y.z * m.z.x))) +
         (m.x.z * ((m.y.x * m.z.y) - (m.y.y * m.z.x)));
}

template <is_scalar T>
constexpr T determinant(Mat4x4<T> const& m) {
    auto mat0 = Mat3x3<T>{ {m.y.y, m.y.z, m.y.w}, {m.z.y, m.z.z, m.z.w}, {m.w.y, m.w.z, m.w.w} };
    auto mat1 = Mat3x3<T>{ {m.y.x, m.y.z, m.y.w}, {m.z.x, m.z.z, m.z.w}, {m.w.x, m.w.z, m.w.w} };
    auto mat2 = Mat3x3<T>{ {m.y.x, m.y.y, m.y.w}, {m.z.x, m.z.y, m.z.w}, {m.w.x, m.w.y, m.w.w} };
    auto mat3 = Mat3x3<T>{ {m.y.x, m.y.y, m.y.z}, {m.z.x, m.z.y, m.z.z}, {m.w.x, m.w.y, m.w.z} };
    T det0 = determinant(mat0);
    T det1 = determinant(mat1);
    T det2 = determinant(mat2);
    T det3 = determinant(mat3);

    return (m.x.x * det0) - (m.x.y * det1) + (m.x.z * det2) - (m.x.w * det3);
}

template<is_scalar T>
constexpr Mat3x3<T> adjugate(Mat3x3<T> const& m) {
  return {
      {(m.y.y * m.z.z) - (m.y.z * m.z.y), (m.x.z * m.z.y) - (m.x.y * m.z.z), (m.x.y * m.y.z) - (m.x.z * m.y.y)},
      {(m.y.z * m.z.x) - (m.y.x * m.z.z), (m.x.x * m.z.z) - (m.x.z * m.z.x), (m.x.z * m.y.x) - (m.x.x * m.y.z)},
      {(m.y.x * m.z.y) - (m.y.y * m.z.x), (m.x.y * m.z.x) - (m.x.x * m.z.y), (m.x.x * m.y.y) - (m.x.y * m.y.x)}
  };
}

template <is_scalar T>
constexpr Mat4x4<T> adjugate(Mat4x4<T> const& m) {
    return {
        {
            determinant(Mat3x3<T>{ {m.y.y, m.y.z, m.y.w}, {m.z.y, m.z.z, m.z.w}, {m.w.y, m.w.z, m.w.w} }),
            -determinant(Mat3x3<T>{ {m.x.y, m.x.z, m.x.w}, {m.z.y, m.z.z, m.z.w}, {m.w.y, m.w.z, m.w.w} }),
            determinant(Mat3x3<T>{ {m.x.y, m.x.z, m.x.w}, {m.y.y, m.y.z, m.y.w}, {m.w.y, m.w.z, m.w.w} }),
            -determinant(Mat3x3<T>{ {m.x.y, m.x.z, m.x.w}, {m.y.y, m.y.z, m.y.w}, {m.z.y, m.z.z, m.z.w} })
        },
        {
            -determinant(Mat3x3<T>{ {m.y.x, m.y.z, m.y.w}, {m.z.x, m.z.z, m.z.w}, {m.w.x, m.w.z, m.w.w} }),
            determinant(Mat3x3<T>{ {m.x.x, m.x.z, m.x.w}, {m.z.x, m.z.z, m.z.w}, {m.w.x, m.w.z, m.w.w} }),
            -determinant(Mat3x3<T>{ {m.x.x, m.x.z, m.x.w}, {m.y.x, m.y.z, m.y.w}, {m.w.x, m.w.z, m.w.w} }),
            determinant(Mat3x3<T>{ {m.x.x, m.x.z, m.x.w}, {m.y.x, m.y.z, m.y.w}, {m.z.x, m.z.z, m.z.w} })
        },
        {
            determinant(Mat3x3<T>{ {m.y.x, m.y.y, m.y.w}, {m.z.x, m.z.y, m.z.w}, {m.w.x, m.w.y, m.w.w} }),
            -determinant(Mat3x3<T>{ {m.x.x, m.x.y, m.x.w}, {m.z.x, m.z.y, m.z.w}, {m.w.x, m.w.y, m.w.w} }),
            determinant(Mat3x3<T>{ {m.x.x, m.x.y, m.x.w}, {m.y.x, m.y.y, m.y.w}, {m.w.x, m.w.y, m.w.w} }),
            -determinant(Mat3x3<T>{ {m.x.x, m.x.y, m.x.w}, {m.y.x, m.y.y, m.y.w}, {m.z.x, m.z.y, m.z.w} })
        },
        {
            -determinant(Mat3x3<T>{ {m.y.x, m.y.y, m.y.z}, {m.z.x, m.z.y, m.z.z}, {m.w.x, m.w.y, m.w.z} }),
            determinant(Mat3x3<T>{ {m.x.x, m.x.y, m.x.z}, {m.z.x, m.z.y, m.z.z}, {m.w.x, m.w.y, m.w.z} }),
            -determinant(Mat3x3<T>{ {m.x.x, m.x.y, m.x.z}, {m.y.x, m.y.y, m.y.z}, {m.w.x, m.w.y, m.w.z} }),
            determinant(Mat3x3<T>{ {m.x.x, m.x.y, m.x.z}, {m.y.x, m.y.y, m.y.z}, {m.z.x, m.z.y, m.z.z} })
        }
    };
}

template<matrix T>
constexpr T inverse(T const& m) {
  auto det = determinant(m);
  if (det == 0) {
    return {};
  }
  return adjugate(m) * (1 / det);
}

template<is_scalar T>
constexpr Mat3x3<T> transpose(Mat3x3<T> const& m) {
  return {
      {m.x.x, m.y.x, m.z.x},
      {m.x.y, m.y.y, m.z.y},
      {m.x.z, m.y.z, m.z.z}
  };
}

template<typename T>
constexpr Mat4x4<T> transpose(Mat4x4<T> const& m) {
  return {
      {m.x.x, m.y.x, m.z.x, m.w.x},
      {m.x.y, m.y.y, m.z.y, m.w.y},
      {m.x.z, m.y.z, m.z.z, m.w.z},
      {m.x.w, m.y.w, m.z.w, m.w.w}
  };
}

template<is_scalar T>
constexpr T dot(Vec2<T> const vec, Vec2<T> const other) {
  auto mul = vec * other;
  return mul.x + mul.y;
}

template<is_scalar T>
constexpr T dot(Vec3<T> const vec, Vec3<T> const other) {
  auto mul = vec * other;
  return mul.x + mul.y + mul.z;
}

template<is_scalar T>
constexpr T dot(Vec4<T> const vec, Vec4<T> const other) {
  auto mul = vec * other;
  return mul.x + mul.y + mul.z + mul.w;
}

template<is_scalar T>
constexpr auto cross(Vec3<T> const& vec1, Vec3<T> const& vec2) -> Vec3<T> {
  return {
    (vec1.y * vec2.z) - (vec1.z * vec2.y),
    (vec1.z * vec2.x) - (vec1.x * vec2.z),
    (vec1.x * vec2.y) - (vec1.y * vec2.x)
  };
}

template<is_scalar T> T cos(T s) { return std::cosf(static_cast<f32>(s)); }

template<is_scalar T> T sin(T s) { return std::sinf(static_cast<f32>(s)); }

template<is_scalar T> T tan(T s) { return std::tanf(static_cast<f32>(s)); }

template<std::floating_point T>
Vec3<T> normalize(Vec3<T> const& v) {
    f32 const inverse_length = 1 / length(v);

    return {v.x * inverse_length, v.y * inverse_length, v.z * inverse_length};
}

template<is_scalar T>
constexpr auto degrees(T const radians) -> T {
    return radians * static_cast<T>(180) / std::numbers::pi_v<T>;
}

template<is_scalar T>
constexpr auto radians(T const degrees) -> T {
  return degrees * (std::numbers::pi_v<T> / static_cast<T>(180));
}

template<is_scalar T>
constexpr auto vec_to_radians(Vec3<T> vec3) -> Vec3<T> {
  return {radians(vec3.x), radians(vec3.y), radians(vec3.z)};
}

template<is_scalar T>
constexpr auto vec_to_radians(Vec4<T> vec4) -> Vec4<T> {
  return {radians(vec4.x), radians(vec4.y), radians(vec4.z), radians(vec4.w)};
}

template<is_scalar T>
constexpr auto vec_to_degrees(Vec3<T> vec3) -> Vec3<T> {
  return {degrees(vec3.x), degrees(vec3.y), degrees(vec3.z)};
}

template<is_scalar T>
constexpr auto vec_to_degrees(Vec4<T> vec4) -> Vec4<T> {
  return {degrees(vec4.x), degrees(vec4.y), degrees(vec4.z), degrees(vec4.w)};
}

template<is_scalar T>
Mat4x4<T> affine_transformation(Vec3<T> const position, Vec3<T> const scale, Vec3<T> const rotation) {
    // Matriz de escala
    Mat4x4<T> scaleMat{
        {scale.x, 0, 0, 0},
        {0, scale.y, 0, 0},
        {0, 0, scale.z, 0},
        {0, 0, 0, 1}
    };

    Mat4x4<T> rotationMat{
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };

    // Traslación
    Mat4x4<T> translationMat{
        {1, 0, 0, position.x},
        {0, 1, 0, position.y},
        {0, 0, 1, position.z},
        {0, 0, 0, 1}
    };

    return translationMat * rotationMat * scaleMat;
}

template<std::floating_point T>
Mat4x4<T> perspective_fov(T const fov, T const aspect_ratio, T const near_plane, T const far_plane) {
    T const tanHalfFov = std::tan(radians(fov) / static_cast<T>(2));

    return Mat4x4<T>{
        {1 / (aspect_ratio * tanHalfFov), 0, 0, 0},
        {0, 1 / tanHalfFov, 0, 0},
        {0, 0, (far_plane + near_plane) / (far_plane - near_plane), (2 * far_plane * near_plane) / (far_plane - near_plane)},
        {0, 0, 1, 0}
    };
}

template<is_scalar T>
Mat4x4<T> look_at(Vec3<T> const position, Vec3<T> const front, Vec3<T> const right, Vec3<T> const up) {

return Mat4x4<T>{
    {right.x, right.y, right.z, -dot(right, position)},
    {up.x, up.y, up.z, -dot(up, position)},
    {front.x, front.y, front.z, -dot(front, position)},
    {0, 0, 0, 1}
};
}

template<is_scalar T>
constexpr T length(Vec2<T> const& v) {
    return std::sqrt((v.x * v.x) + (v.y * v.y));
}

template<is_scalar T>
constexpr T length(Vec3<T> const& v) {
    return std::sqrt((v.x * v.x) + (v.y * v.y) + v.z * v.z);
}

template<is_scalar T>
constexpr T length(Vec4<T> const& v) {
    return std::sqrt((v.x * v.x) + v.y * v.y + (v.z * v.z) + v.w * v.w);
}

template<is_scalar T>
constexpr auto translation(Mat4x4<T> mat) -> Vec3<T> {
    return {mat.x.w, mat.y.w, mat.z.w};
}

template<is_scalar T>
constexpr auto scale(Mat4x4<T> mat) -> Vec3<T> {
    return {length(mat.x), length(mat.y), length(mat.z)};
}

template<is_scalar T>
constexpr auto rotation(Mat4x4<T> mat) -> Vec3<T> {
    Mat3x3<T> rotMat{Vec3<T>{mat.x}, Vec3<T>{mat.y}, Vec3<T>{mat.z}};

    T pitch = std::atan2(rotMat.z.y, rotMat.z.z);
    T yaw = std::atan2(-rotMat.z.x, std::sqrt(rotMat.z.y * rotMat.z.y + rotMat.z.z * rotMat.z.z));
    T roll = std::atan2(rotMat.y.x, rotMat.x.x);

    return {pitch, yaw, roll};
}

scalar squaredDistance(vector auto const v_i, vector auto const v_j){
    const scalar x = v_i.x - v_j.x;
    const scalar y = v_i.y - v_j.y;
    const scalar z = v_i.z - v_j.z;
    return std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2);
}






// clang-format on

} // namespace reveal3d::math
