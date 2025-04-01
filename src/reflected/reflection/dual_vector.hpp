/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file dual_vector.hpp
 * @version 1.0
 * @date 14/03/2025
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include <iostream>
#include <ranges>

#include "concepts.hpp"
#include "iterator.hpp"
#include "utils.hpp"

namespace acb {

template<has_proxy T, memory_layout Layout = layout::aos, template<typename> class Alloc = std::allocator>
class dual_vector {
public:
  // ********** Type traits **********
  using value_type           = T;
  using underlying_container = typename Layout::template vector<T, Alloc>;
  using view_type            = typename T::template proxy_type<dual_vector>;
  using memory_layout        = Layout;
  using iterator             = ProxyIterator<view_type>;

  // ********** Constructors **********
  constexpr dual_vector() = default;

  constexpr dual_vector(std::initializer_list<value_type> init)
    requires(soa_layout<memory_layout>)
  {
    for (auto const& item: init) {
      push_back(item);
    }
  }

  constexpr dual_vector(std::initializer_list<value_type> init)
    requires(aos_layout<memory_layout>)
    : data_(init) { }

  // ********** Member functions **********

  constexpr view_type at(std::size_t index) { return {data_, index}; }

  constexpr view_type operator[](std::size_t index) { return {data_, index}; }

  constexpr view_type front() { return {data_, 0}; }

  constexpr view_type back() { return {data_, size() - 1}; }

  template<typename Self>
  constexpr auto begin(this Self&& self) {
    return iterator {std::forward<Self>(self).data_, 0};
  }

  template<typename Self>
  constexpr auto end(this Self&& self) {
    return iterator {std::forward<Self>(self).data_, std::forward<Self>(self).size() - 1};
  }


  // ********** SOA member functions **********

  constexpr void push_back(value_type const& item)
    requires(soa_layout<memory_layout>)
  {
    template for (constexpr auto member: data_member_array(^^underlying_container)) {
      data_.[:member:].push_back(item.[:nonstatic_data_member<value_type>(identifier_of(member)):]);
    }
  }

  constexpr void push_back(view_type const value)
    requires(soa_layout<memory_layout>)
  {
    // auto underlying_value = *value;
    // constexpr auto zip = static_zip(
        // nonstatic_data_members_of(^^underlying_container), nonstatic_data_members_of(type_of(^^underlying_value))
    // );

    // template for (constexpr auto member: zip) {
      // data_.[:std::get<0>(member):].push_back(underlying_value.[:std::get<1>(member):]);
    // }
  }

  constexpr std::size_t size() const
    requires(soa_layout<memory_layout>)
  {
    return data_.[:nonstatic_data_member<underlying_container>(0):].size();
  }

  // ********** AOS member functions **********

  constexpr void push_back(value_type const& item)
    requires(aos_layout<memory_layout>)
  {
    data_.push_back(item);
  }

  constexpr void push_back(view_type const view)
    requires(aos_layout<memory_layout>)
  {
    data_.push_back(*view);
  }

  constexpr std::size_t size() const
    requires(aos_layout<memory_layout>)
  {
    return data_.size();
  }

  // ********** Operators **********

  friend constexpr bool operator==(dual_vector const& vec1, dual_vector const& vec2)
    requires(aos_layout<memory_layout>)
  {
    return vec1.data_ == vec2.data_;
  }

  friend constexpr bool operator==(dual_vector const& vec1, dual_vector const& vec2)
    requires(soa_layout<memory_layout>)
  {
    bool equal = true;
    template for (constexpr auto member: data_member_array(^^underlying_container)) {
      equal &= (vec1.data_.[:member:] == vec2.data_.[:member:]);
    };
    return equal;
  }

private:
  underlying_container data_;
};


// --- Deducing guidelines  ---
template<has_proxy T>
dual_vector(std::initializer_list<T> init) -> dual_vector<T>;


} // namespace acb
