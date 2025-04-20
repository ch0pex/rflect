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
  using value_type           = T;
  using underlying_container = typename Layout::template array<T, N>;
  using view_type            = typename T::template proxy_type<dual_array>;
  using const_view_type      = typename T::template proxy_type<dual_array const>;
  using memory_layout        = Layout;
  using iterator             = proxy_iterator<view_type>;
  using const_iterator       = proxy_iterator<const_view_type>;

  // ********** Constructors **********
  constexpr dual_array() = default;

  constexpr dual_array(std::initializer_list<value_type> init)
    requires(soa_layout<Layout>)
    : data_(init) { }

  constexpr dual_array(std::initializer_list<value_type> init)
    requires(aos_layout<Layout>)
    : data_() {
    std::ranges::copy(init, std::ranges::begin(data_));
  }

  // ********** Member functions **********

  constexpr view_type at(std::size_t const index) { return {data_, index}; }

  [[nodiscard]] constexpr const_view_type at(std::size_t const index) const { return const_view_type {data_, index}; }

  constexpr view_type operator[](std::size_t const index) { return {data_, index}; }

  constexpr const_view_type operator[](std::size_t const index) const { return {data_, index}; }

  constexpr view_type front() { return {data_, 0}; }

  constexpr view_type back() { return {data_, size() - 1}; }

  constexpr auto begin() { return iterator {data_, 0}; }

  constexpr auto end() { return iterator {data_, size()}; }

  constexpr auto begin() const { return const_iterator {data_, 0}; }

  constexpr auto end() const { return const_iterator {data_, size()}; }

  constexpr auto cbegin() const { return const_iterator {data_, 0}; }

  constexpr auto cend() const { return const_iterator {data_, size()}; }

  [[nodiscard]] constexpr std::size_t size() const { return data_.size(); }

  [[nodiscard]] constexpr std::size_t max_size() const { return data_.max_size(); }

  [[nodiscard]] constexpr std::size_t empty() const { return data_.empty(); }
private:
  underlying_container data_;
};

} // namespace rflect
