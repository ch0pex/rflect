/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file struct_to_tuple.hpp
 * @version 1.0
 * @date 4/4/25
 * @brief Struct to tuple
 *
 * Struct to tuple converter functions
 */

#pragma once

#include <experimental/meta>

namespace rflect {

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

template<typename From>
constexpr auto struct_to_tuple(From const& from) {
  return detail::get_struct_to_tuple_helper<From>()(from);
}

}