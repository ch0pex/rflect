/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file concepts.hpp
 * @version 1.0
 * @date 3/6/2025
 * @brief  Math module concepts
 *
 */

#pragma once

#include "utils/primitive_types.hpp"
#include <concepts>

namespace sim::math {

template<typename T>
concept is_scalar = std::integral<T> or std::floating_point<T>;

struct ScalarOperators {
  template<typename Self>
  constexpr Self operator+=(this Self& self, is_scalar auto scalar) {
    self += Self{scalar};
    return self;
  }

  template<typename Self>
  constexpr Self operator-=(this Self& self, is_scalar auto scalar) {
    self -= Self{scalar};
    return self;
  }

  template<typename Self>
  constexpr Self operator*=(this Self& self, is_scalar auto scalar) {
    self *= Self{scalar};
    return self;
  }

  template<typename Self>
  constexpr Self operator/=(this Self& self, is_scalar auto scalar) {
    self /= Self{scalar};
    return self;
  }
};

using scalar = f64;

} // namespace reveal3d::math