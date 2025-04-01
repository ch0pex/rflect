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

namespace acb {

template<typename T>
class ProxyIterator {
public:
  using iterator_category = std::forward_iterator_tag;
  using difference_type   = std::size_t;
  using value_type        = T;
  using reference         = T&;
  using pointer           = T*;
  using container = typename T::underlying_container;

  ProxyIterator(container& cont, std::size_t const index) : index_(index), container_(cont) { }

  constexpr ProxyIterator operator++() {
    ++index_;
    return ProxyIterator {container_, index_};
  }

  constexpr ProxyIterator operator++(int) {
    ProxyIterator old = *this;
    operator++();
    return old;
  }

  constexpr value_type operator*() { return {container_, index_}; }

  friend constexpr bool operator==(ProxyIterator const& proxy1, ProxyIterator const& proxy2) {
    return proxy1.index_ == proxy2.index_ and
           std::addressof(proxy1.container_) == std::addressof(proxy2.container_);
  }

  friend constexpr bool operator!=(ProxyIterator const& proxy1, ProxyIterator const& proxy2) {
    return not(proxy1 == proxy2);
  }

private:
  std::size_t index_;
  container& container_;

};

} // namespace acb
