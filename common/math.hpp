/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file math.hpp
 * @version 1.0
 * @date 12/03/2025
 * @brief Math library
 *
 * Math structures using static reflection to autogenerate
 * multiple dimension vectors
 */

#pragma once

#include "math_detail.hpp"

namespace sim::math {

// *** Scalar type ***
using scalar = f32;

// *** Vector concept ***
template<typename T>
concept is_vector = detail::is_vector<T>::value;

// *** Algebra Vectors ***
template<u8 Dimension, std::floating_point T = scalar, std::ranges::range auto Names = detail::vector_components>
using vec = detail::Vec<Dimension, T, Names>;

using vec2 = vec<2>;
using vec3 = vec<3>;
using vec4 = vec<4>;

// *** RGB and RGBA vectors ***
using color3 = vec<3, scalar, detail::color_components>;
using color4 = vec<4, scalar, detail::color_components>;

// *** Texture vectors ***
using texture2d = vec<2, scalar, detail::texture_components>;
using texture3d = vec<3, scalar, detail::texture_components>;

} // namespace sim::math
