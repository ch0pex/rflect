/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details) 
 ************************************************************************/
/**
 * @file memory_layout.hpp
 * @version 1.0
 * @date 28/03/2025
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include <rflect/converters/struct_to_soa.hpp>

#include <array>
#include <vector>

namespace rflect::layout {

struct aos {
  template<typename T, std::size_t N>
  using array = std::array<T, N>;

  template<typename T, template<class> class Alloc>
  using vector = std::vector<T, Alloc<T>>;
};

struct soa {
  template<class T, std::size_t N>
  using array = struct_of_arrays<T, N>;

  template<class T, template<class> class Alloc>
  using vector = struct_of_vectors<T, Alloc>;
};

} // namespace layout
