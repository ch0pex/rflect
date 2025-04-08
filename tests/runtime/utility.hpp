/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file utility.hpp
 * @version 1.0
 * @date 4/7/25
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include <rflect/containers.hpp>

struct Mock {
  DEFINE_PROXY(id, density, velocity);

  std::int32_t id;
  std::double_t density;
  std::array<std::double_t, 3> velocity;
};

constexpr bool operator==(Mock const& a, Mock const& b) {
  return a.id == b.id and a.density == b.density and a.velocity == b.velocity;
}

template<typename C>
constexpr bool operator==(Mock::proxy_type<C> const& a, Mock const& b) {
  return a.id() == b.id and a.density() == b.density and a.velocity() == b.velocity;
}

template<typename C>
constexpr bool operator==(Mock const& a, Mock::proxy_type<C> const& b) {
  return b == a;
}

template<typename C>
constexpr bool operator==(Mock::proxy_type<C> const& a, Mock::proxy_type<C> const& b) {
  return a.id() == b.id() and a.density() == b.density() and a.velocity() == b.velocity();
}

constexpr Mock mock_0 {.id = 0, .density = 12.15, .velocity = {1.0, 2.0, 3.0}};
constexpr Mock mock_1 {.id = 1, .density = 13.15, .velocity = {0.0}};
constexpr Mock mock_2 {.id = 2, .density = 14.15, .velocity = {1.0, 2.0, 3.0}};
constexpr Mock mock_3 {.id = 3, .density = 15.15, .velocity = {1.0, 2.0, 3.0}};
