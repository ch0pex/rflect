/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file layout_concepts.hpp
 * @version 1.0
 * @date 3/6/2025
 * @brief Short description
 *
 * Longer description
 */

#pragma once


#include "scalar.hpp"


namespace sim::math {

template<is_scalar T>
struct Vec2;

template<is_scalar T>
struct Vec3;

template<is_scalar T>
struct Vec4;

template<is_scalar T>
struct Mat3x3;

template<is_scalar T>
struct Mat4x4;

namespace detail {

template<typename T>
struct is_vector : std::false_type { };

template<is_scalar T>
struct is_vector<Vec2<T>> : std::true_type { };

template<is_scalar T>
struct is_vector<Vec3<T>> : std::true_type { };

template<is_scalar T>
struct is_vector<Vec4<T>> : std::true_type { };

template<typename T>
struct is_matrix : std::false_type { };

template<is_scalar T>
struct is_matrix<Mat3x3<T>> : std::true_type { };

template<is_scalar T>
struct is_matrix<Mat4x4<T>> : std::true_type { };
} // namespace detail

template<typename T>
concept vector = detail::is_vector<T>::value;

template<typename T>
concept matrix = detail::is_matrix<T>::value;

template<typename T>
concept algebra_type = detail::is_vector<T>::value or detail::is_matrix<T>::value or is_scalar<T>;

} // namespace sim::math
