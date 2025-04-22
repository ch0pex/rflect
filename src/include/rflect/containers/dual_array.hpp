/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dual_array.hpp
 * @version 1.0
 * @date 4/4/25
 * @brief DualArray class
 */
#pragma once

#include <rflect/concepts/layout_concepts.hpp>
#include <rflect/concepts/proxy_concepts.hpp>
#include <rflect/containers/iterator.hpp>


namespace rflect {

template<has_proxy T, std::size_t N, memory_layout Layout = layout::aos>
class dual_array {
public:
  /**********************************
   *          Member types          *
   **********************************/
  using value_type           = T;
  using underlying_container = typename Layout::template array<T, N>;
  using view_type            = typename T::template proxy_type<dual_array>;
  using const_view_type      = typename T::template proxy_type<dual_array const>;
  using memory_layout        = Layout;
  using iterator             = proxy_iterator<view_type>;
  using const_iterator       = proxy_iterator<const_view_type>;
  using size_type            = std::size_t;
  using difference_type      = std::ptrdiff_t;

  /**********************************
   *        Member functions        *
   **********************************/

  // ********* Constructors *********

  constexpr dual_array() = default;

  constexpr dual_array(std::initializer_list<value_type> init)
    requires(soa_layout<Layout>)
    : data_(init) { }

  constexpr dual_array(std::initializer_list<value_type> init)
    requires(aos_layout<Layout>)
    : data_() {
    std::ranges::copy(init, std::ranges::begin(data_));
  }

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

  [[nodiscard]] constexpr size_type size() const noexcept { return data_.size(); }

  [[nodiscard]] constexpr size_type max_size() const noexcept { return data_.size(); }

  [[nodiscard]] constexpr size_type empty() const noexcept { return data_.empty(); }

private:
  underlying_container data_ {};
};

} // namespace rflect
