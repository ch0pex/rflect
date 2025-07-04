/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file iterator.hpp
 * @version 1.0
 * @date 29/03/2025
 * @brief Proxy iterator class
 *
 * This class iterates over a dual structure building proxy views of
 * its elements
 */

#pragma once

#include <rflect/concepts/layout_concepts.hpp>

#include <iterator>

namespace rflect {

/**
 * @brief Iterator for iterating over components in dual_vector and dual_array containers.
 *
 * This iterator allows iteration over the elements of a `dual_vector` or `dual_array`, independently of whether
 * they are using a Structure of Arrays (SoA) or Array of Structures (AoS) layout.
 * It provides standard iterator functionality to access container elements through a proxy type.
 *
 * @tparam ViewType Proxy type used to access elements in the container.
 */
template<typename ViewType>
class proxy_iterator {
public:
  using iterator_category = std::forward_iterator_tag;
  using difference_type   = std::ptrdiff_t;
  using value_type        = ViewType;
  using reference         = ViewType&;
  using pointer           = ViewType*;
  using container         = typename ViewType::underlying_container;

  proxy_iterator() = default;

  proxy_iterator(container& container, std::size_t const index) : index_(index), container_(&container) { }

  constexpr proxy_iterator& operator++() {
    ++index_;
    return *this;
  }

  constexpr proxy_iterator operator++(int) {
    proxy_iterator old = *this;
    ++(*this);
    return old;
  }

  template<typename Self>
  constexpr value_type operator*(this Self&& self) {
    return {*self.container_, self.index_};
  }

  friend constexpr proxy_iterator operator+(proxy_iterator const& proxy1, difference_type const index) {
    return {*proxy1.container_, proxy1.index_ + index};
  }

  friend constexpr proxy_iterator operator-(proxy_iterator const& proxy1, difference_type const index) {
    return {*proxy1.container_, proxy1.index_ - index};
  }

  friend constexpr difference_type operator-(proxy_iterator const& proxy1, proxy_iterator const& proxy2) {
    return proxy1.index_ - proxy2.index_;
  }

  friend constexpr bool operator==(proxy_iterator const& proxy1, proxy_iterator const& proxy2) {
    return proxy1.index_ == proxy2.index_ and proxy1.container_ == proxy2.container_;
  }

private:
  std::size_t index_ {};
  container* container_;
};

} // namespace rflect
