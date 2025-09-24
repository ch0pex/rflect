/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file rflect3d.hpp
 * @version 1.0
 * @date 5/31/25
 * @brief Rflect3d
 *
 * Rflect3d entry point
 */

#pragma once

#include <rflect/rflect.hpp>

namespace rflect3d {

namespace detail {
template<class... T>
struct EcsBuilder {
  struct impl;

  consteval {
    // clang-format off
    std::tuple<T...> args;
    std::vector<std::meta::info> old_members = nonstatic_data_members_of(^^T);
    std::vector<std::meta::info> new_members = {};

    for (std::meta::info member: old_members) {
      auto allocator = substitute(^^Alloc, { type_of(member) });
      auto array_type = substitute(^^std::vector, { type_of(member), allocator });
      auto mem_descr = data_member_spec(array_type, {.name = identifier_of(member)});
      new_members.push_back(mem_descr);
    }

    define_aggregate(^^impl, new_members);
    // clang-format on
  }
};

} // namespace detail

// template<typename... Components>
// using systems = typename detail::EcsBuilder<Components...>::impl;
//
// template<typename Entity, typename... Components>
// struct Ecs {
//   systems<Components...> systems;
// };


} // namespace rflect3d
