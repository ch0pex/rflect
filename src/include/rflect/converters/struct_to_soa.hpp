/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file struct_to_soa.hpp
 * @version 1.0
 * @date 28/03/2025
 * @brief Struct to Soa
 *
 * Struct to struct of arrays/vectors converters using
 * static reflection (p2996R10)
 */

#pragma once

#include <experimental/meta>

namespace rflect {

namespace detail {

template<typename T, size_t N>
struct struct_of_arrays {
  struct impl;

  consteval {
    std::vector<std::meta::info> old_members = nonstatic_data_members_of(^^T, std::meta::access_context::unchecked());
    std::vector<std::meta::info> new_members = {};
    for (std::meta::info member: old_members) {
      auto array_type = substitute(
          ^^std::array,
          {
            type_of(member), std::meta::reflect_constant(N)
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
    // clang-format off
    std::vector<std::meta::info> old_members = nonstatic_data_members_of(^^T, std::meta::access_context::unchecked());
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

template<class T>
struct struct_of_pointers {
  struct impl;

  consteval {
    // clang-format off
    std::vector<std::meta::info> old_members = nonstatic_data_members_of(^^T, std::meta::access_context::unchecked());
    std::vector<std::meta::info> new_members = {};

    for (std::meta::info member: old_members) {
      auto pointer_type = add_pointer(type_of(member));
      auto mem_descr = data_member_spec(pointer_type, {.name = identifier_of(member)});
      new_members.push_back(mem_descr);
    }

    define_aggregate(^^impl, new_members);
    // clang-format on
  }
};

} // namespace detail

/**
 * @brief Type alias that generates a structure of pointers`s from a given struct type.
 *
 * For a given struct type `T`, this alias produces a new struct where each member is
 * replaced with a `std::vector` of the corresponding type. This allows storing multiple
 * instances of `T` in a structure-of-arrays (SoA) layout, which is often more cache-friendly.
 *
 * @tparam T The struct type to be transformed.
 * @tparam Alloc Allocator template to be used for each vector (defaults to `std::allocator`).
 */
template<typename T, template<class> class Alloc = std::allocator>
using struct_of_pointers = typename detail::struct_of_vectors<T, Alloc>::impl;

/**
 * @brief Type alias that generates a structure of `std::vector`s from a given struct type.
 *
 * For a given struct type `T`, this alias produces a new struct where each member is
 * replaced with a `std::vector` of the corresponding type. This allows storing multiple
 * instances of `T` in a structure-of-arrays (SoA) layout, which is often more cache-friendly.
 *
 * @tparam T The struct type to be transformed.
 * @tparam Alloc Allocator template to be used for each vector (defaults to `std::allocator`).
 */
template<typename T, template<class> class Alloc = std::allocator>
using struct_of_vectors = typename detail::struct_of_vectors<T, Alloc>::impl;

/**
 * @brief Type alias that generates a structure of `std::array`s from a given struct type.
 *
 * Similar to `struct_of_vectors`, this alias transforms a struct `T` into a new struct where
 * each member becomes a `std::array` of fixed size `N`. This is useful for stack-allocated
 * structures with SoA layout when the size is known at compile time.
 *
 * @tparam T The struct type to be transformed.
 * @tparam N The fixed size of the arrays.
 */
template<typename T, std::size_t N>
using struct_of_arrays = typename detail::struct_of_arrays<T, N>::impl;

} // namespace rflect
