/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file test_dual_vector.cpp
 * @version 1.0
 * @date 28/03/2025
 * @brief Short description
 *
 * Longer description
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "test_containers.hpp"

using namespace rflect;

constexpr std::size_t array_size = 50U;

template<typename Layout>
using container = dual_array<Mock, array_size, Layout>;

TEST_SUITE_BEGIN("Dual Array");

TEST_CASE_TEMPLATE("Constructor with initializer list", T, layout::aos, layout::soa) {
  container<T> mock_vector {mock_0, mock_1, mock_2};

  CHECK(mock_vector.size() == array_size);
}

TEST_CASE_TEMPLATE("Element access", T, layout::aos, layout::soa) { test_element_access<container<T>>(); }

TEST_CASE_TEMPLATE("Capacity", T, layout::aos, layout::soa) {
  dual_array<Mock, 0, T> container_0;
  container<T> container_1 {mock_0, mock_1};
  container<T> container_2 {mock_0, mock_1, mock_2};

  SUBCASE("size") {
    CHECK(container_0.size() == 0);
    CHECK(container_1.size() == array_size);
    CHECK(container_2.size() == array_size);
  }

  SUBCASE("max_size") {
    CHECK(container_0.max_size() == 0);
    CHECK(container_1.max_size() == array_size);
    CHECK(container_2.max_size() == array_size);
  }

  SUBCASE("empty") {
    CHECK(container_0.empty() == true);
    CHECK(container_1.empty() == false);
    CHECK(container_2.empty() == false);
  }
}

TEST_CASE_TEMPLATE("Iterators", T, layout::aos, layout::soa) {
  static constexpr auto size = 4;
  dual_array<Mock, size> mock {mock_0, mock_1, mock_2, mock_3};

  std::int32_t i  = 0;
  std::double_t j = 12.15;

  CHECK(mock.size() == size);

  SUBCASE("begin - end") {
    for (auto it = mock.begin(); it != mock.end(); ++it) {
      CHECK((*it).id() == i++);
      CHECK((*it).density() == j++);
    }
  }

  SUBCASE("cbegin - cend") {
    for (auto it = mock.cbegin(); it != mock.cend(); ++it) {
      CHECK((*it).id() == i++);
      CHECK((*it).density() == j++);
    }
  }
}

TEST_CASE_TEMPLATE("Ranges", T, layout::aos, layout::soa) {
  static constexpr std::size_t size = 4;
  dual_array<Mock, size> mock {mock_0, mock_1, mock_2, mock_3};

  std::int32_t i  = 0;
  std::double_t j = 12.15;

  CHECK(mock.size() == size);

  for (auto elem: mock) {
    CHECK(elem.id() == i++);
    CHECK(elem.density() == j++);
  }
}

TEST_SUITE_END();
