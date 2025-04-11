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

#include <rflect/concepts/converter_closure.hpp>

#include <experimental/meta>

namespace rflect {

namespace detail {

consteval auto as_tuple_type(std::meta::info type) {
  return substitute(
      ^^std::tuple, //
      nonstatic_data_members_of(type) //
          | std::views::transform(std::meta::type_of) //
          | std::views::transform(std::meta::remove_cvref) //
          | std::ranges::to<std::vector>() //
  );
}

template<typename To, typename From, std::meta::info... members>
constexpr auto struct_to_tuple_helper(From const& from) -> To {
  return To(from.[:members:]...);
}

template<typename From, typename To>
consteval auto get_struct_to_tuple_helper() {

  std::vector args = {^^To, ^^From};
  for (auto mem: nonstatic_data_members_of(^^From)) {
    args.push_back(reflect_value(mem));
  }

  return extract<To (*)(From const&)>(substitute(^^struct_to_tuple_helper, args));
}

template<typename From>
struct struct_to_tuple_fn : converter_closure<struct_to_tuple_fn<From>> {
  using To = [:as_tuple_type(^^From):];
  constexpr auto operator()(From const& from) { return get_struct_to_tuple_helper<From, To>()(from); }
};

} // namespace detail

template<typename From>
using as_tuple = [:detail::as_tuple_type(^^From):];

inline constexpr auto struct_to_tuple = []<typename From>(From && from) {
  static constexpr auto fn = detail::struct_to_tuple_fn<From>{};
  return fn(from);
};
// inline constexpr auto to_struct = detail::struct_to_tuple_fn {};

} // namespace rflect
