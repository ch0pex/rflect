/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file type_array.hpp
 * @version 1.0
 * @date 8/15/2025
 * @brief Types Array
 *
 * Read only array of types, can retrieve types or reflections of that types
 */

#pragma once

#include <array>

namespace rflect {

template<typename... Types> // clang-format off
class type_array { // clang-format on
public:
  using size_type = std::size_t;

  type_array() : data {^^Types...} { }

  template<std::size_t N>
  using at = Types...[N];

  auto at(std::size_t const index) -> std::meta::info { return data.at(index); }

  auto begin() { return data.begin(); }

  auto end() { return data.end(); }

  size_type size() { return data.size(); }

private:
  std::array<std::meta::info, sizeof...(Types)> data;
};

// template<std::size_t N> // clang-format off
// class type_array { // clang-format on
// public:
//   using size_type = std::size_t;
//
//   template<typename... Types>
//   type_array() : data {^^Types...} { }
//
//   // template<std::size_t N>
//   // using at = ;
//
//   auto at(std::size_t const index) -> std::meta::info { return data.at(index); }
//
//   auto begin() { return data.begin(); }
//
//   auto end() { return data.end(); }
//
//   size_type size() { return data.size(); }
//
// private:
//   std::array<std::meta::info, sizeof...(Types)> data;
// };

} // namespace rflect
