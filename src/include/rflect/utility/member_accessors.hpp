/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file member_accessors.hpp
 * @version 1.0
 * @date 4/4/25
 * @brief Accessors utilities
 *
 * This functions allows the user to access structs/class
 * members in new ways, utilizing static reflection.
 */
#pragma once
#include "static_array.hpp"

namespace rflect {
template<typename T>
consteval auto nonstatic_data_member(std::size_t const number) {
  return std::meta::nonstatic_data_members_of(^^T)[number];
}

template<typename T>
consteval auto function_member(std::size_t n) {
  constexpr auto member_functions = members_of(^^T) | std::views::filter(std::meta::is_function) | to_static_array();
  return member_functions[n];
}

template<typename T>
consteval auto nonstatic_data_member(std::string_view const name) {
  template for (constexpr auto field: nonstatic_data_members_of(^^T) | to_static_array()) {
    if (has_identifier(field) && identifier_of(field) == name)
      return field;
  }
  throw std::invalid_argument("No such nonstatic data member");
}

} // namespace rflect
