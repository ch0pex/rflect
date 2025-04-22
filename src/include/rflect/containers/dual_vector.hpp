/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dual_vector.hpp
 * @version 1.0
 * @date 14/03/2025
 * @brief dual_vector class definition
 *
 * DualVector offers the user soa and aos data
 * layout under the same interface with zero overhead
 * thanks to the new C++26 feature, static reflection.
 *
 */

#pragma once

#include <rflect/concepts/layout_concepts.hpp>
#include <rflect/concepts/proxy_concepts.hpp>
#include <rflect/containers/iterator.hpp>

namespace rflect {

template<has_proxy T, memory_layout Layout = layout::aos, template<typename> class Alloc = std::allocator>
class dual_vector {
public:
  /**********************************
   *          Member types          *
   **********************************/
  using value_type           = T;
  using underlying_container = typename Layout::template vector<T, Alloc>;
  using view_type            = typename T::template proxy_type<dual_vector>;
  using const_view_type      = typename T::template proxy_type<dual_vector const>;
  using memory_layout        = Layout;
  using iterator             = proxy_iterator<view_type>;
  using const_iterator       = proxy_iterator<const_view_type>;
  using size_type            = std::size_t;
  using difference_type      = std::ptrdiff_t;

  /**********************************
   *        Member functions        *
   **********************************/

  // ********* Constructors *********

  constexpr dual_vector() = default;

  constexpr dual_vector(std::initializer_list<value_type> init) : data_(init) { }

  constexpr explicit dual_vector(size_type size) : data_(size) { }

  // ********* Element access *********

  constexpr view_type at(size_type const index) { return {data_, index}; }

  [[nodiscard]] constexpr const_view_type at(size_type const index) const { return const_view_type {data_, index}; }

  constexpr view_type operator[](size_type const index) { return {data_, index}; }

  constexpr const_view_type operator[](size_type const index) const { return {data_, index}; }

  constexpr view_type front() { return {data_, 0}; }

  constexpr view_type back() { return {data_, size() - 1}; }

  // ********* Iterators *********

  constexpr iterator begin() noexcept { return {data_, 0}; }

  constexpr iterator end() noexcept { return {data_, size()}; }

  [[nodiscard]] constexpr const_iterator begin() const noexcept { return {data_, 0}; }

  [[nodiscard]] constexpr const_iterator end() const noexcept { return {data_, size()}; }

  [[nodiscard]] constexpr const_iterator cbegin() const noexcept { return {data_, 0}; }

  [[nodiscard]] constexpr const_iterator cend() const noexcept { return {data_, size()}; }

  // ********* Capacity *********

  [[nodiscard]] constexpr size_type empty() const noexcept { return data_.size(); }

  [[nodiscard]] constexpr size_type size() const noexcept { return data_.size(); }

  [[nodiscard]] constexpr size_type max_size() const noexcept { return data_.size(); }

  [[nodiscard]] constexpr size_type capacity() const noexcept { return data_.size(); }

  // ********* Modifiers *********

  constexpr void push_back(value_type const& item) { data_.push_back(item); }

  constexpr void push_back(view_type const view) { data_.push_back(*view); }

  // ********** Operators **********

  friend constexpr bool operator==(dual_vector const& vec1, dual_vector const& vec2) {
    return vec1.data_ == vec2.data_;
  }

private:
  underlying_container data_ {};
};

/**********************************
 *        Deduction guides        *
 **********************************/
template<has_proxy T>
dual_vector(std::initializer_list<T> init) -> dual_vector<T>;

} // namespace rflect
