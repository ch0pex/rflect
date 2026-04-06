/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file to_static.hpp
 * @version 1.0
 * @date 4/4/25
 * @brief Static array utilitites
 *
 * These utilities are workarounds that are needed by now due to the
 * proposal implementation status
 */

#pragma once

#include <rflect/concepts/converter_closure.hpp>

#include <meta>
#include <ranges>

namespace rflect {

template<std::size_t N>
consteval auto static_iota() {
  return std::views::iota(0ZU, N);
}

consteval auto operator""_ss(char const* str, [[maybe_unused]] size_t length) -> char const* {
  std::string_view const sv {str, length};
  return std::define_static_string(sv);
}

struct to_static_fn : converter_closure<to_static_fn> {
  template<std::ranges::input_range R>
  consteval auto operator()(R&& r) const {
    return define_static_array(r);
  }
};

inline constexpr auto to_static_array = to_static_fn {};

} // namespace rflect
