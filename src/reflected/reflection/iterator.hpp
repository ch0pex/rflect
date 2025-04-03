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

#include <iterator>
#include "concepts.hpp"

namespace acb {

template<typename ViewType>
class ProxyIterator {
public:
  using iterator_category = std::forward_iterator_tag;
  using difference_type   = std::ptrdiff_t;
  using value_type        = ViewType;
  using reference         = ViewType&;
  using pointer           = ViewType*;
  using container         = typename ViewType::underlying_container;

  ProxyIterator() = default;

  ProxyIterator(container& container, std::size_t const index) : index_(index), container_(&container) { }

  constexpr ProxyIterator& operator++() {
    ++index_;
    return *this;
  }

  constexpr ProxyIterator operator++(int) {
    ProxyIterator old = *this;
    ++(*this);
    return old;
  }

  template<typename Self>
  constexpr value_type operator*(this Self&& self) {
    return {*self.container_, self.index_};
  }

  friend constexpr bool operator==(ProxyIterator const& proxy1, ProxyIterator const& proxy2) {
    return proxy1.index_ == proxy2.index_ and std::addressof(proxy1.container_) == std::addressof(proxy2.container_);
  }

private:
  std::size_t index_;
  container* container_;
};

} // namespace acb
