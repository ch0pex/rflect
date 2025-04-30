/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file comparison.hpp
 * @version 1.0
 * @date 4/22/25
 * @brief Comparison operators
 */
#pragma once

#include <rflect/containers/dual_array.hpp>
#include <rflect/containers/dual_vector.hpp>

namespace rflect {

template<typename T, std::size_t N>
constexpr bool operator==(multi_array<T, N> const& array1, multi_array<T, N> const& array2) {
  auto const sz = array1.size();
  return sz == array2.size() && std::ranges::equal(array1, array2);
}

template<typename T, template<typename> class Alloc>
constexpr bool operator==(multi_vector<T, Alloc> const& vec1, multi_vector<T, Alloc> const& vec2) {
  auto const sz = vec1.size();
  return sz == vec2.size() && std::ranges::equal(vec1, vec2);
}

template<has_proxy T, memory_layout Layout, template<typename> class Alloc>
constexpr bool operator==(dual_vector<T, Layout, Alloc> const& vec1, dual_vector<T, Layout, Alloc> const& vec2) {
  return vec1.data_ == vec2.data_;
}

template<has_proxy T, std::size_t N>
constexpr bool operator==(dual_array<T, N> const& array1, dual_array<T, N> const& array2) {
  return array1.data_ == array2.data_;
}

} // namespace rflect
