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

#include <cassert>


#include "concepts.hpp"
#include "utils.hpp"

namespace acb {

// template<has_proxy T>
// class Iterator {
// public:
//   using value_type        = T;
//   using iterator_category = std::forward_iterator_tag;
//   using difference_type   = std::size_t;
//   using reference         = T&;
//   using pointer           = T*;
//
//   constexpr Iterator operator++() { return ++value_; }
//
//   constexpr Iterator operator++(int) {
//     Iterator old = *this;
//     operator++();
//     return old;
//   }
//
//   constexpr reference operator*() { return value_; }
//
//   constexpr reference operator->() { return &value_; }
//
//   friend constexpr bool operator==(Iterator const& proxy1, Iterator const& proxy2) {
//     return proxy1.value_ == proxy2.value_;
//   }
//
//   friend constexpr bool operator!=(Iterator const& proxy1, Iterator const& proxy2) { return not(proxy1 == proxy2); }
//
// private:
//   value_type value_;
// };
//

template<has_proxy T, std::size_t N, memory_layout Layout = layout::aos>
class dual_array {
  // TODO
};

template<has_proxy T, memory_layout Layout = layout::aos, template<typename> class Alloc = std::allocator>
class dual_vector {
public:
  // *** Type traits ***
  using value_type           = T;
  using underlying_container = typename Layout::template vector<T, Alloc>;
  using view_type            = typename T::template proxy_type<dual_vector>;
  using memory_layout        = Layout;
  using iterator             = view_type;

  // *** Constructors ***
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


  // *** SOA member functions ***
  constexpr void push_back(value_type const& item)
    requires(soa_layout<memory_layout>)
  {
    template for (constexpr auto member: data_member_array(^^underlying_container)) {
      data_.[:member:].push_back(item.[:member_named<value_type>(identifier_of(member)):]);
    };
  }

  constexpr void push_back(view_type value)
    requires(soa_layout<memory_layout>)
  { }

  constexpr std::size_t size()
    requires(soa_layout<memory_layout>)
  {
    return data_.[:member_number<underlying_container>(0):].size();
  }

  friend constexpr bool operator==(dual_vector const& vec1, dual_vector const& vec2)
    requires(soa_layout<memory_layout>)
  {
    bool equal = true;
    template for (constexpr auto member: data_member_array(^^underlying_container)) {
      equal &= (vec1.[:member:] == vec2.[:member_named<value_type>(identifier_of(member)):]);
    };
    return equal;
  }

  // *** AOS member functions ***
  constexpr std::size_t size()
    requires(aos_layout<memory_layout>)
  {
    return data_.size();
  }

  constexpr void push_back(value_type const& item)
    requires(aos_layout<memory_layout>)
  {
    data_.push_back(item);
  }

  constexpr void push_back(view_type value)
    requires(aos_layout<memory_layout>)
  {
    // data_.push_back(value);
  }

  constexpr auto begin()
    requires(aos_layout<memory_layout>)
  {
    return view_type {data_, 0}; // TODO
  }

  constexpr auto end()
    requires(aos_layout<memory_layout>)
  {
    return view_type {data_, data_.size()}; // TODO
  }

  // *** Member functions ***
  constexpr view_type at(std::size_t index) {
    assert(index < size());
    return {data_, index};
  }

  constexpr view_type operator[](std::size_t index) {
    assert(index < size());
    return {data_, index};
  }

  friend constexpr bool operator==(dual_vector const& vec1, dual_vector const& vec2)
    requires(aos_layout<memory_layout>)
  {
    return vec1.data_ == vec2.data_;
  }

private:
  underlying_container data_;
};


template<has_proxy T>
dual_vector(std::initializer_list<T> init) -> dual_vector<T>;


} // namespace acb
