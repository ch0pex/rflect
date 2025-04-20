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

constexpr Mock mock_0 {.id = 0, .density = 12.15, .velocity = {1.0, 2.0, 3.0}};
constexpr Mock mock_1 {.id = 1, .density = 13.15, .velocity = {0.0}};
constexpr Mock mock_2 {.id = 2, .density = 14.15, .velocity = {1.0, 2.0, 3.0}};
constexpr Mock mock_3 {.id = 3, .density = 15.15, .velocity = {1.0, 2.0, 3.0}};

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

template<typename Container>
void test_element_access() {
  Container mock {mock_0, mock_1, mock_2};

  CHECK(mock.at(0) == mock_0);
  CHECK(mock.at(1) == mock_1);
  CHECK(mock.at(2) == mock_2);

  SUBCASE("at") {
    auto mock_view = mock.at(0);
    mock_view.id() = 1;

    CHECK(mock_view.id() == mock_1.id);
  }

  SUBCASE("front") {
    auto mock_view   = mock.front();
    auto mock_view_2 = mock.at(0);

    CHECK(mock_view == mock_view_2);
  }

  SUBCASE("back") {
    auto mock_view   = mock.back();
    auto mock_view_2 = mock.at(mock.size() - 1);

    CHECK(mock_view == mock_view_2);
  }

  SUBCASE("operator[]") {
    CHECK(mock[0] == mock_0);
    CHECK(mock[1] == mock_1);
    CHECK(mock[2] == mock_2);
  }
}
