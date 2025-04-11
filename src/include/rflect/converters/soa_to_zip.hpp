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

#include <ranges>

namespace rflect {

namespace detail {

template<typename From>
consteval auto as_zip_type() -> std::meta::info {
  // clang-format off
  constexpr auto raw_members = nonstatic_data_members_of(^^From)
                               | std::views::transform(std::meta::type_of)
                               | std::views::transform(std::meta::remove_cvref)
                               | to_static_array;

  std::vector<std::meta::info> value_members;

  template for (constexpr auto member: raw_members) {
    std::meta::info new_member;
    if constexpr (std::is_const_v<std::remove_reference_t<From>>) {
      new_member = add_lvalue_reference(^^typename[:member:] const);
    }
    else {
      new_member = add_lvalue_reference(^^typename[:member:]);
    }

    value_members.push_back(substitute(^^std::views::all_t, { new_member }));
  }

  return substitute(^^std::ranges::zip_view, value_members);
}

template<typename To, typename From, std::meta::info... members>
constexpr auto soa_to_zip_helper(From&& from) -> To {
  auto value = std::views::zip(std::forward<From>(from).[:members:]...);
  return value;
}

template<typename From, typename To>
consteval auto get_soa_to_zip_helper() {

  std::vector args = {^^To, ^^From};
  for (auto mem: nonstatic_data_members_of(^^std::remove_reference_t<From>)) {
    args.push_back(reflect_value(mem));
  }

  return extract<To (*)(From&&)>(substitute(^^soa_to_zip_helper, args));
}

/**
 * Calls std::ranges::zip over all 'From' non-static data members
 * @tparam From Struct of ranges type
 * @param from Struct to be converted
 * @return From represented as zip_view of its members
 */
template<typename From>
struct soa_to_zip_fn : converter_closure<soa_to_zip_fn<From>> {
  using To = [:as_zip_type<std::remove_reference_t<From>>():];

  constexpr auto operator()(From&& from) const {
    return detail::get_soa_to_zip_helper<From, To>()(std::forward<From>(from));
  }

};

} // namespace detail

template<typename From>
using as_zip = typename detail::soa_to_zip_fn<From>::To;

constexpr auto soa_to_zip = []<typename From>(From&& from) {
  static constexpr auto fn = detail::soa_to_zip_fn<From>{};
  return fn(std::forward<From>(from));
};

} // namespace rflect
