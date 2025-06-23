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

template<typename Layout>
using container = dual_vector<Mock, Layout>;

TEST_SUITE_BEGIN("Dual Vector");

TEST_CASE_TEMPLATE("Constructor with initializer list", T, layout::aos, layout::soa) {
  container<T> mock_vector {mock_0, mock_1, mock_2};

  CHECK(mock_vector.size() == 3U);
}

TEST_CASE_TEMPLATE("push_back", T, layout::aos, layout::soa) {
  container<T> mock_vector {mock_0, mock_1, mock_2};
  container<T> mock_vector_2;
  mock_vector_2.push_back(mock_0);
  mock_vector_2.push_back(mock_1);
  mock_vector_2.push_back(mock_2);

  CHECK(mock_vector == mock_vector_2);
  mock_vector_2.push_back(mock_2);
  CHECK(mock_vector != mock_vector_2);

  auto mock_view = mock_vector_2.back();
  mock_vector.push_back(mock_view);
  CHECK(mock_vector == mock_vector_2);
}

TEST_CASE_TEMPLATE("Comparison", T, layout::aos, layout::soa) {
  container<T> vec1 {mock_0, mock_1, mock_2, mock_3};
  container<T> vec2 {mock_0, mock_1, mock_2, mock_3};
  container<T> vec3 {mock_0, mock_1, mock_2};

  CHECK(vec1 == vec2);
  CHECK(vec1 != vec3);
}

TEST_CASE_TEMPLATE("erase", T, layout::aos, layout::soa) {
  container<T> vec1 {mock_0, mock_1, mock_2, mock_3};
  container<T> vec2 {mock_0, mock_1, mock_2, mock_3};
  container<T> vec3 {mock_0, mock_1, mock_2};

  auto iterator = vec1.erase(vec1.begin() + 3);
  CHECK(iterator == vec1.end());
  CHECK(vec1 == vec3);
  CHECK(vec1 != vec2);

  vec2.erase(vec3.begin() + 3);
  CHECK(vec1 == vec2);
}

TEST_CASE_TEMPLATE("pop_back", T, layout::aos, layout::soa) {
  container<T> vec1 {mock_0, mock_1, mock_2, mock_3};
  container<T> vec2 {mock_0, mock_1, mock_2, mock_3};
  container<T> vec3 {mock_0, mock_1, mock_2};

  vec1.pop_back();
  CHECK(vec1 == vec3);
  CHECK(vec1 != vec2);
  vec2.pop_back();
  CHECK(vec1 == vec2);
}

TEST_CASE_TEMPLATE("Accessors", T, layout::aos, layout::soa) { test_element_access<container<T>>(); }

TEST_CASE_TEMPLATE("operator[]", T, layout::aos, layout::soa) {
  container<T> mock {mock_0, mock_1, mock_2};

  CHECK(mock[0] == mock_0);
  CHECK(mock[1] == mock_1);
  CHECK(mock[2] == mock_2);
}

TEST_CASE_TEMPLATE("size", T, layout::aos, layout::soa) {
  container<T> vec_0;
  container<T> vec_1 {mock_0, mock_1};
  container<T> vec_2 {mock_0, mock_1, mock_2};

  CHECK(vec_0.size() == 0U);
  CHECK(vec_1.size() == 2U);
  CHECK(vec_2.size() == 3U);
}

TEST_CASE_TEMPLATE("Vector proxy iterator", T, layout::aos, layout::soa) {
  container<T> vec {mock_0, mock_1, mock_2, mock_3};
  constexpr auto vec_size = 4;

  std::int32_t i  = 0;
  std::double_t j = 12.15;

  CHECK(vec.size() == vec_size);

  for (auto it = vec.begin(); it != vec.end(); ++it) {
    CHECK((*it).id() == i++);
    CHECK((*it).density() == j++);
  }
}

TEST_CASE_TEMPLATE("range iteration", T, layout::aos, layout::soa) {
  container<T> vec {mock_0, mock_1, mock_2, mock_3};
  constexpr auto vec_size = 4;

  std::int32_t i  = 0;
  std::double_t j = 12.15;

  CHECK(vec.size() == vec_size);

  for (auto elem: vec) {
    CHECK(elem.id() == i++);
    CHECK(elem.density() == j++);
  }
}


TEST_SUITE_END();
