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

}

struct Transform {

};

struct Material {

};

struct Mesh {

};

struct Light {

};

struct Script {

};


struct Components {
 rflect::dual_vector<Transform> transform;
 rflect::dual_vector<Material> material;
 rflect::dual_vector<Mesh> mesh;
 rflect::dual_vector<Light> light;
 rflect::dual_vector<Script> script;
};

template<class ...T>
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

template<typename ...Components>
using Ecs =


}