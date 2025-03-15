/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file utils.hpp
 * @version 1.0
 * @date 01/11/2024
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include "vector.hpp"

#include <array>

namespace sim::math::utils {

template<typename T>
auto to_array(Vec2<T> v) { return std::array {v.x, v.y}; }

template<typename T>
auto to_array(Vec3<T> v) { return std::array {v.x, v.y, v.z}; }

template<typename T>
auto to_array(Vec4<T> v) { return std::array {v.x, v.y, v.z, v.w}; }

} // namespace reveal3d::math::utl
