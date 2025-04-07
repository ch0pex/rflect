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

namespace rflect {
template<typename T>
consteval auto nonstatic_data_member(std::size_t const number) {
  return std::meta::nonstatic_data_members_of(^^T)[number];
}

consteval auto data_member_array(std::meta::info info) { return define_static_array(nonstatic_data_members_of(info)); }

consteval auto member_array(std::meta::info info) { return define_static_array(members_of(info)); }

consteval auto member_array(std::meta::info info, auto&& filter) {
  return define_static_array(members_of(info) | std::views::filter(filter));
}

template<typename T>
consteval auto function_member(std::size_t n) {
  constexpr auto member_functions = member_array(^^T, std::meta::is_function);
  return member_functions[n];
}

template<typename T>
consteval auto nonstatic_data_member(std::string_view const name) {
  template for (constexpr auto field: data_member_array(^^T)) {
    if (has_identifier(field) && identifier_of(field) == name)
      return field;
  }
  throw std::invalid_argument("No such nonstatic data member");
}

}
