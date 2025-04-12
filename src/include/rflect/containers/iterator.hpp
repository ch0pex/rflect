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

  friend constexpr bool operator==(proxy_iterator const& proxy1, proxy_iterator const& proxy2) {
    // TODO compare std::addressof(proxy1.container.begin()) == std::addressof(proxy2.container.begin())
    return proxy1.index_ == proxy2.index_;
  }

private:
  std::size_t index_{};
  container* container_;
};

} // namespace rflect::
