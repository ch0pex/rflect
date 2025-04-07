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

#include <rflect/utility/static_array.hpp>

namespace rflect {

template<typename T>
consteval auto type_soa_zipview() {

  constexpr auto raw_members = nonstatic_data_members_of(^^T) | std::views::transform(std::meta::type_of) |
                               std::views::transform(std::meta::remove_cvref) | to_static_array();

  std::vector<std::meta::info> value_members;

  template for (constexpr auto member: raw_members) {
    auto new_member = add_lvalue_reference(^^typename[:member:] ::value_type const);
    // auto const_member = add_const(new_member);
    value_members.push_back(new_member);
  }

  return substitute(^^std::tuple, value_members);
}

template<typename To, typename From, std::meta::info... members>
constexpr auto soa_to_zip_helper(From const& from, std::size_t const index) -> To {

  auto value = std::views::zip(from.[:members:]...);
  return value[index];
}

template<typename From>
consteval auto get_soa_to_zip_helper() {
  using To = [:type_soa_zipview<From>():];

  std::vector args = {^^To, ^^From};
  for (auto mem: nonstatic_data_members_of(^^From)) {
    args.push_back(reflect_value(mem));
  }

  return extract<To (*)(From const&, std::size_t const index)>(substitute(^^soa_to_zip_helper, args));
}

template<typename From>
constexpr auto soa_to_zip(From const& from, std::size_t const index) {
  return get_soa_to_zip_helper<From>()(from, index);
}

}