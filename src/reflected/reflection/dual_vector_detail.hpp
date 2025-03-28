/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details) 
 ************************************************************************/
/**
 * @file dual_vector_detail.hpp
 * @version 1.0
 * @date 28/03/2025
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include <experimental/meta>

namespace acb::detail {

template<typename T, size_t N>
struct struct_of_arrays {
  struct impl;

  consteval {
    std::vector<std::meta::info> old_members = nonstatic_data_members_of(^^T);
    std::vector<std::meta::info> new_members = {};
    for (std::meta::info member: old_members) {
      auto array_type = substitute(
          ^^std::array,
          {
            type_of(member), std::meta::reflect_value(N)
          }
      );
      auto mem_descr = data_member_spec(array_type, {.name = identifier_of(member)});
      new_members.push_back(mem_descr);
    }

    define_aggregate(^^impl, new_members);
  }
};

template<class T, template<class> class Alloc>
struct struct_of_vectors {
  struct impl;

  consteval {
    std::vector<std::meta::info> old_members = nonstatic_data_members_of(^^T);
    std::vector<std::meta::info> new_members = {};

    for (std::meta::info member: old_members) {
      auto allocator = substitute(
          ^^Alloc,
          {
            type_of(member)
          }
      );
      auto array_type = substitute(
          ^^std::vector,
          {
            type_of(member), allocator
          }
      );
      auto mem_descr = data_member_spec(array_type, {.name = identifier_of(member)});
      new_members.push_back(mem_descr);
    }

    define_aggregate(^^impl, new_members);
  }
};

}