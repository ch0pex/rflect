/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file enum.hpp
 * @version 1.0
 * @date 4/11/25
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include <experimental/meta>

namespace rflect {

template <typename E>
  requires std::is_enum_v<E>
constexpr std::string enum_to_string(E value) {
  template for (constexpr auto e : std::meta::enumerators_of(^^E)) {
    if (value == [:e:]) {
      return std::string(std::meta::identifier_of(e));
    }
  }

  return "<unnamed>";
}


}