/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details) 
 ************************************************************************/
/**
 * @file reflection_utils.hpp
 * @version 1.0
 * @date 12/03/2025
 * @brief Reflection utils
 *
 * Static Reflection utilities
 */

#pragma once

#include <experimental/meta>

namespace acb {

template<typename T>
consteval auto member_number(std::size_t const number) {
  return std::meta::nonstatic_data_members_of(^^T)[number];
}

consteval auto data_member_array(std::meta::info info) {
  return define_static_array(nonstatic_data_members_of(info));
}

template<typename T>
consteval auto member_named(std::string_view const name) {
  template for (constexpr auto field : data_member_array(^^T)) {
    if (has_identifier(field) && identifier_of(field) == name)
      return field;
  }
  throw std::invalid_argument("No such member");
}

consteval auto operator""_ss(const char* str, [[maybe_unused]] size_t length) -> const char *  {
    return std::meta::define_static_string(str);
}

}