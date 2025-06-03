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

namespace rflect {

template<std::size_t N>
consteval auto static_iota() {
  return define_static_array(template_arguments_of(underlying_entity_of(^^std::make_index_sequence<N>)) | std::views::drop(1));
}

consteval auto operator""_ss(char const* str, [[maybe_unused]] size_t length) -> char const* {
  return std::define_static_string(str);
}

struct to_static_fn : converter_closure<to_static_fn>  {
  template <std::ranges::input_range R>
  consteval auto operator()(R&& r) const {
    return define_static_array(r);
  }
};

inline constexpr auto to_static_array = to_static_fn{};

} // namespace rflect
