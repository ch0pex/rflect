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

#include "utils/primitive_types.hpp"

#include <experimental/meta>

namespace sim {

template<typename T>
consteval auto member_number(u64 number) {
  return std::meta::nonstatic_data_members_of(^^T)[number];
}

template<typename T>
consteval auto member_named(std::string_view name) {
  for (std::meta::info field : nonstatic_data_members_of(^^T)) {
    if (has_identifier(field) && identifier_of(field) == name)
      return field;
  }
  return std::meta::info(); // This may return something but don't know
}

consteval auto operator""_ss(const char* str, size_t len) -> const char *  {
    return std::meta::define_static_string(str);
}

}