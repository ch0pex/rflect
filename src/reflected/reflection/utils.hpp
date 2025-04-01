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
#include <ranges>
#include <concepts>

namespace acb {

namespace detail {

consteval auto type_struct_to_tuple(std::meta::info type) {
  return substitute(
      ^^std::tuple, nonstatic_data_members_of(type) | std::views::transform(std::meta::type_of) |
                        std::views::transform(std::meta::remove_cvref) | std::ranges::to<std::vector>()
  );
}

template<typename To, typename From, std::meta::info... members>
constexpr auto struct_to_tuple_helper(From const& from) -> To {
  return To(from.[:members:]...);
}

template<typename From>
consteval auto get_struct_to_tuple_helper() {
  using To = [:type_struct_to_tuple(^^From):];

  std::vector args = {^^To, ^^From};
  for (auto mem: nonstatic_data_members_of(^^From)) {
    args.push_back(reflect_value(mem));
  }

  return extract<To (*)(From const&)>(substitute(^^struct_to_tuple_helper, args));
}

} // namespace detail

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

consteval auto operator""_ss(char const* str, [[maybe_unused]] size_t length) -> char const* {
  return std::meta::define_static_string(str);
}

template<typename From>
constexpr auto struct_to_tuple(From const& from) {
  return detail::get_struct_to_tuple_helper<From>()(from);
}

template<typename... Args>
  requires(sizeof...(Args) >= 2)
constexpr auto static_zip(Args... args) {
  return define_static_array(std::views::zip(args...));
}

} // namespace acb
