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

#include "utility.hpp"

using namespace rflect;

TEST_SUITE_BEGIN("Dual Vector");

TEST_CASE_TEMPLATE("Constructor with initializer list", T, layout::aos, layout::soa) {
  dual_vector<Mock, T> mock_vector {mock_0, mock_1, mock_2};

  CHECK(mock_vector.size() == 3U);
}

TEST_CASE_TEMPLATE("push_back", T, layout::aos, layout::soa) {
  dual_vector<Mock, T> mock_vector {mock_0, mock_1, mock_2};
  dual_vector<Mock, T> mock_vector_2;
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

TEST_CASE_TEMPLATE("Accessors", T, layout::aos, layout::soa) {
  dual_vector<Mock, T> mock {mock_0, mock_1, mock_2};

  CHECK(mock.at(0) == mock_0);
  CHECK(mock.at(1) == mock_1);
  CHECK(mock.at(2) == mock_2);

  SUBCASE("at") {
    auto mock_view = mock.at(0);
    mock_view.id() = 1;

    CHECK(mock_view.id() == mock_1.id);
  }

  SUBCASE("front") {
    auto mock_view = mock.front();
    auto mock_view_2 = mock.at(0);

    CHECK(mock_view == mock_view_2);
  }

  SUBCASE("back") {
    auto mock_view = mock.back();
    auto mock_view_2 = mock.at(mock.size() - 1);

    CHECK(mock_view == mock_view_2);
  }

}

TEST_CASE_TEMPLATE("operator[]", T, layout::aos, layout::soa) {
  dual_vector<Mock, T> mock {mock_0, mock_1, mock_2};

  CHECK(mock[0] == mock_0);
  CHECK(mock[1] == mock_1);
  CHECK(mock[2] == mock_2);
}

TEST_CASE_TEMPLATE("size", T, layout::aos, layout::soa) {
  dual_vector<Mock, T> vec_0;
  dual_vector<Mock, T> vec_1 {mock_0, mock_1};
  dual_vector<Mock, T> vec_2 {mock_0, mock_1, mock_2};

  CHECK(vec_0.size() == 0U);
  CHECK(vec_1.size() == 2U);
  CHECK(vec_2.size() == 3U);
}

TEST_CASE_TEMPLATE("Vector proxy iterator", T, layout::aos, layout::soa) {
  dual_vector<Mock, T> vec {mock_0, mock_1, mock_2, mock_3};
  constexpr auto vec_size = 4;

  std::int32_t i = 0;
  std::double_t j = 12.15;

  CHECK(vec.size() == vec_size);

  for (auto it = vec.begin(); it != vec.end(); ++it) {
    CHECK((*it).id() == i++);
    CHECK((*it).density() == j++);
  }
}

TEST_CASE_TEMPLATE("range iteration", T, layout::aos, layout::soa) {
  dual_vector<Mock, T> vec {mock_0, mock_1, mock_2, mock_3};
  constexpr auto vec_size = 4;

  std::int32_t i = 0;
  std::double_t j = 12.15;

  CHECK(vec.size() == vec_size);

  for (auto elem : vec) {
    CHECK(elem.id() == i++);
    CHECK(elem.density() == j++);
  }
}

TEST_SUITE_END();
