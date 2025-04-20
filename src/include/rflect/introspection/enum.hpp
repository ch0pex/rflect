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

/**
 * @brief Converts an enum value to its corresponding name as a string.
 *
 * @tparam E The enum type to introspect. Must satisfy `std::is_enum_v<E>`.
 * @param value The enum value to be converted to a string.
 * @return The name of the enumerator as a `std::string`, or "<unnamed>" if no match is found.
 */
template<typename E>
  requires std::is_enum_v<E>
constexpr std::string enum_to_string(E value) {
  template for (constexpr auto e: std::meta::enumerators_of(^^E)) {
    if (value == [:e:]) {
      return std::string(std::meta::identifier_of(e));
    }
  }

  return "<unnamed>";
}

} // namespace rflect
