/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file memory_layout.hpp
 * @version 1.0
 * @date 28/03/2025
 * @brief Memory layout aliases
 */

#pragma once

#include <rflect/containers/multi_vector.hpp>
#include <rflect/converters/struct_to_soa.hpp>

#include <array>
#include <vector>


namespace rflect::layout {

/**
 * @brief Layout for Array of Structures (AoS).
 *
 * The `aos` structure defines the types used for an Array of Structures (AoS) layout, where data is stored
 * as a collection of structures, each containing all the members of the aggregate type.
 */
struct aos {
  template<typename T, std::size_t N>
  using array = std::array<T, N>;

  template<typename T, template<class> class Alloc>
  using vector = std::vector<T, Alloc<T>>;
};

/**
 * @brief Layout for Structure of Arrays (SoA).
 *
 * The `soa` structure defines the types used for a Structure of Arrays (SoA) layout, where data is stored
 * as separate arrays for each member of the aggregate type, allowing for more efficient data processing
 * in certain scenarios (e.g., SIMD operations).
 */
struct soa {
  template<class T, std::size_t N>
  using array = struct_of_arrays<T, N>;

  template<class T, template<class> class Alloc>
  using vector = multi_vector<T, Alloc>;
};

} // namespace rflect::layout
