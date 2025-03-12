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

#include "primitive_types.hpp"

#include <experimental/meta>

namespace sim {

/// *** Implementation, this will be needed till expansions are fully implemented
namespace detail {
  template<auto... vals>
  struct replicator_type {
    template<typename F>
      constexpr void operator>>(F body) const {
        (body.template operator()<vals>(), ...);
      }
  };

  template<auto... vals>
  replicator_type<vals...> replicator = {};
}

template<typename R>
consteval auto expand(R range) {
  std::vector<std::meta::info> args;
  for (auto r : range) {
    args.push_back(reflect_value(r));
  }
  return substitute(^^detail::replicator, args);
}

/************ This will work fine when expansion statements are finished
    constexpr view at(std::size_t index) {
        view value;
        template for (constexpr std::meta::info member : nonstatic_data_members_of(^view)) {
            value.[:member:] = &data_.[:member_named<container>(identifier_of(member)):];
        }
        return value;
    }

    constexpr void push_back(T const& item) {
        template for (constexpr std::meta::info member : nonstatic_data_members_of(^container)) {
            data_.[:member:].push_back(item.[:member_named<T>(identifier_of(member)):]);
        }
    }
    */

/// *** Meanwhile will have to use this workaround ***
#define template_for(elem_name, elements)  [:expand(elements):] >> [&]<auto elem_name>

template<typename T>
consteval auto member_number(u64 number) {
  return std::meta::nonstatic_data_members_of(^^T)[number];
}

template<typename T>
consteval auto member_named(std::string_view name) {
  for (std::meta::info field : nonstatic_data_members_of(^^T)) {
    if (has_identifier(field) && identifier_of(field) == name)
      return field;
  }
  return std::meta::info(); // This may return something but don't know
}

consteval auto operator""_ss(const char* str, size_t len) -> const char *  {
    return std::meta::define_static_string(str);
}

}