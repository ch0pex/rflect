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
      nonstatic_data_members_of(type, std::meta::access_context::unchecked()) //
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
  for (auto mem: nonstatic_data_members_of(^^From, std::meta::access_context::unchecked())) {
    args.push_back(reflect_constant(mem));
  }

  return extract<To (*)(From const&)>(substitute(^^struct_to_tuple_helper, args));
}

struct struct_to_tuple_fn : converter_closure<struct_to_tuple_fn> {

  template<typename FromType>
  constexpr auto operator()(FromType const& from) {
    using To = [:as_tuple_type(^^FromType):];
    return get_struct_to_tuple_helper<FromType, To>()(from);
  }
};

} // namespace detail

/**
 * @brief Type alias that maps a struct type to its corresponding tuple type.
 *
 * This alias uses metaprogramming to deduce a `std::tuple` type representing
 * the member types of a given struct.
 *
 * @tparam From Struct type to be converted to a tuple of types.
 */
template<typename From>
using as_tuple = [:detail::as_tuple_type(^^From):];

/**
 * @brief Converts a struct instance into a tuple of its member values.
 *
 * This function takes a struct and returns a `std::tuple` containing the values
 * (or references, depending on value category) of its members, preserving their declaration order.
 *
 * @tparam From Type of the struct to convert.
 * @param from The struct instance to convert.
 * @return A tuple containing the member values (or references) of the given struct.
 */
inline constexpr auto struct_to_tuple = detail::struct_to_tuple_fn {};

} // namespace rflect
