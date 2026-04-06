/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file soa_to_zip.hpp
 * @version 1.0
 * @date 4/4/25
 * @brief Soa to Zip
 *
 * Struct of arrays to zip converter utility
 */

#pragma once

#include <rflect/converters/to_static.hpp>

#include <meta>
#include <ranges>
#include <source_location>

namespace rflect {

namespace detail {

constexpr std::string to_string_constexpr(unsigned int value) {
  if (value == 0)
    return "0";
  std::string res;
  while (value > 0) {
    res += static_cast<char>('0' + (value % 10));
    value /= 10;
  }
  std::reverse(res.begin(), res.end());
  return res;
}

template<typename From>
consteval auto as_zip_type() -> std::meta::info try {
  // clang-format off
  static constexpr auto raw_members = nonstatic_data_members_of(^^From, std::meta::access_context::current())
                               | std::views::transform(std::meta::type_of)
                               | std::views::transform(std::meta::remove_cvref)
                               | to_static_array;

  std::vector<std::meta::info> value_members;
  static_assert(raw_members.size() > 0);

  template for (constexpr auto member: raw_members) {
    std::meta::info new_member;
    if constexpr (std::is_const_v<std::remove_reference_t<From>>) {
      using member_type = typename[:member:] const;
      new_member = add_lvalue_reference(^^member_type);
    }
    else {
      using member_type = typename[:member:];
      new_member = add_lvalue_reference(^^member_type);
    }

    value_members.push_back(substitute(^^std::views::all_t, { new_member }));
  }

  return substitute(^^std::ranges::zip_view, value_members);
} catch(std::meta::exception const& e) {
  using std::string_literals::operator""s;
  std::source_location sl = e.where();
  std::string error_message = "Error in as_zip_type for type "s + identifier_of(^^From) + "in line "s + to_string_constexpr(sl.line()) + ": "s + e.what();
  throw std::meta::exception(error_message, ^^as_zip_type);
}

template<typename To, typename From, std::meta::info... members>
constexpr auto soa_to_zip_helper(From&& from) -> To {
  auto value = std::views::zip(std::forward<From>(from).[:members:]...);
  return value;
}

template<typename From, typename To>
consteval auto get_soa_to_zip_helper() {

  std::vector args = {^^To, ^^From};
  for (auto mem: nonstatic_data_members_of(^^std::remove_reference_t<From>, std::meta::access_context::unchecked())) {
    args.push_back(reflect_constant(mem));
  }

  return extract<To (*)(From&&)>(substitute(^^soa_to_zip_helper, args));
}

/**
 * Calls std::ranges::zip over all 'From' non-static data members
 * @tparam From Struct of ranges type
 * @param from Struct to be converted
 * @return From represented as zip_view of its members
 */
struct soa_to_zip_fn : converter_closure<soa_to_zip_fn> {

  template<typename From>
  constexpr auto operator()(From&& from) const {
    using To = [:as_zip_type<std::remove_reference_t<From>>():];
    static constexpr auto fn = detail::get_soa_to_zip_helper<From, To>();
    return fn(std::forward<From>(from));
  }

};

} // namespace detail

/**
 * @brief Type alias that transforms a structure of arrays into a zip-compatible type.
 *
 * This alias deduces the resulting type when converting a structure-of-arrays (SoA) layout
 * into a zipped form (typically a view of tuples, one per element), enabling element-wise
 * iteration as if it were an array-of-structures (AoS).
 *
 * @tparam From The structure-of-arrays type to convert.
 */
template<typename From>
using as_zip = [:detail::as_zip_type<std::remove_reference_t<From>>():];

/**
 * @brief Converts a structure-of-arrays into a zipped view.
 *
 * This function transforms a struct containing arrays or vectors (SoA layout)
 * into a zip-like structure that allows iteration as if it were an array of structs,
 * where each element is a tuple of references to the corresponding fields.
 *
 * @tparam From The type of the structure-of-arrays.
 * @param from The SoA instance to convert.
 * @return A zipped view that enables element-wise access in AoS fashion.
 */
inline constexpr auto soa_to_zip = detail::soa_to_zip_fn{};

} // namespace rflect
