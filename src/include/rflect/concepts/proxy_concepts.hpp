/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file proxy_concepts.hpp
 * @version 1.0
 * @date 4/7/25
 * @brief Proxy class concepts
 */

#pragma once

#include <concepts>
#include <experimental/meta>


namespace rflect {

template<typename T>
concept has_proxy = requires(T element) {
  requires std::is_aggregate_v<T>;
  // TODO
  // Restrict to structs with at least 1 nonstatic data member
};

} // namespace rflect
