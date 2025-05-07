/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file struct.hpp
 * @version 1.0
 * @date 4/11/25
 * @brief Short description
 *
 * Longer description
 */
#pragma once

#pragma once

#include <rflect/converters/to_static.hpp>

#include <functional>

namespace rflect {

/**
 * @brief Retrieves the non-static data member at the specified index of the given type.
 *
 * @tparam T The type to introspect.
 * @param index The index of the data member to retrieve.
 * @return A metadata object representing the non-static data member at the given index.
 */
template<typename T>
consteval auto nonstatic_data_member(std::size_t const index) {
  static constexpr auto members = std::meta::nonstatic_data_members_of(^^T) | to_static_array;
  if (index < members.size()) {
    return members[index];
  }
  throw std::invalid_argument("No such nonstatic data member");
}

/**
 * @brief Retrieves the member function with the specified identifier of the given type.
 *
 * @tparam T The type to introspect.
 * @param identifier The identifier of the data member to retrieve.
 * @return A metadata object representing the non-static data member at the given index.
 */
template<typename T>
consteval auto nonstatic_data_member(std::string_view const identifier) {
  template for (constexpr auto field: nonstatic_data_members_of(^^T) | to_static_array) {
    if (has_identifier(field) && identifier_of(field) == identifier)
      return field;
  }
  throw std::invalid_argument("No such nonstatic data member");
}

/**
 * @brief Retrieves the member function at the specified index of the given type.
 *
 * @tparam T The type to introspect.
 * @param index The index of the member function to retrieve.
 * @return A metadata object representing the member function at the given index.
 */
template<typename T>
consteval auto member_function(std::size_t index) {
  constexpr auto member_functions = members_of(^^T) //
                                    | std::views::filter(std::meta::is_function) //
                                    | to_static_array;
  if (index < member_functions.size()) {
    return member_functions[index];
  }
  throw std::invalid_argument("No such member function");
}

template<typename T, typename Filter>
consteval auto members(Filter filter = {}) {
  return members_of(^^T) | std::views::filter(filter) | to_static_array;
}

template<typename T, typename Filter = std::identity>
consteval auto member_count(Filter filter = {}) {
  auto members = members_of(^^T) | std::views::filter(filter);
  return members.size();
}

} // namespace rflect
