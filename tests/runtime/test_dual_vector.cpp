/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file test_dual_vector.cpp
 * @version 1.0
 * @date 28/03/2025
 * @brief Comprehensive tests for dual_vector
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "test_containers.hpp"

using namespace rflect;

template<typename Layout>
using container = dual_vector<Mock, Layout>;

TEST_SUITE_BEGIN("Dual Vector");

// *** Constructors ***

TEST_CASE_TEMPLATE("Default constructor", T, layout::aos, layout::soa) {
  container<T> vec;
  CHECK(vec.size() == 0U);
}

TEST_CASE_TEMPLATE("Explicit size constructor", T, layout::aos, layout::soa) {
  container<T> vec(4);
  CHECK(vec.size() == 4U);
}

TEST_CASE_TEMPLATE("Initializer list constructor", T, layout::aos, layout::soa) {
  container<T> vec {mock_0, mock_1, mock_2};
  CHECK(vec.size() == 3U);
}

TEST_CASE("Deduction guide") {
  dual_vector vec {mock_0, mock_1, mock_2};
  CHECK(vec.size() == 3U);
  CHECK(vec[0] == mock_0);
}

// *** Capacity ***

TEST_CASE_TEMPLATE("size", T, layout::aos, layout::soa) {
  container<T> vec_0;
  container<T> vec_1 {mock_0, mock_1};
  container<T> vec_2 {mock_0, mock_1, mock_2};

  CHECK(vec_0.size() == 0U);
  CHECK(vec_1.size() == 2U);
  CHECK(vec_2.size() == 3U);
}

TEST_CASE_TEMPLATE("empty on default-constructed vector", T, layout::aos, layout::soa) {
  container<T> vec;
  CHECK(vec.size() == 0U);
}

TEST_CASE_TEMPLATE("empty after push_back", T, layout::aos, layout::soa) {
  container<T> vec;
  vec.push_back(mock_0);
  CHECK(vec.size() == 1U);
}

// *** Element access ***

TEST_CASE_TEMPLATE("Accessors", T, layout::aos, layout::soa) { test_element_access<container<T>>(); }

TEST_CASE_TEMPLATE("operator[]", T, layout::aos, layout::soa) {
  container<T> vec {mock_0, mock_1, mock_2};

  CHECK(vec[0] == mock_0);
  CHECK(vec[1] == mock_1);
  CHECK(vec[2] == mock_2);
}

TEST_CASE_TEMPLATE("front", T, layout::aos, layout::soa) {
  container<T> vec {mock_0, mock_1, mock_2};

  CHECK(vec.front() == mock_0);
  CHECK(vec.front() == vec.at(0));
}

TEST_CASE_TEMPLATE("back", T, layout::aos, layout::soa) {
  container<T> vec {mock_0, mock_1, mock_2};

  CHECK(vec.back() == mock_2);
  CHECK(vec.back() == vec.at(2));
}

TEST_CASE_TEMPLATE("front and back on single-element vector", T, layout::aos, layout::soa) {
  container<T> vec {mock_1};

  CHECK(vec.front() == mock_1);
  CHECK(vec.back() == mock_1);
  CHECK(vec.front() == vec.back());
}

TEST_CASE_TEMPLATE("at reads all fields", T, layout::aos, layout::soa) {
  container<T> vec {mock_0, mock_1, mock_2, mock_3};

  SUBCASE("id field") {
    CHECK(vec.at(0).id() == mock_0.id);
    CHECK(vec.at(1).id() == mock_1.id);
    CHECK(vec.at(2).id() == mock_2.id);
    CHECK(vec.at(3).id() == mock_3.id);
  }

  SUBCASE("density field") {
    CHECK(vec.at(0).density() == mock_0.density);
    CHECK(vec.at(1).density() == mock_1.density);
    CHECK(vec.at(2).density() == mock_2.density);
    CHECK(vec.at(3).density() == mock_3.density);
  }

  SUBCASE("velocity field") {
    CHECK(vec.at(0).velocity() == mock_0.velocity);
    CHECK(vec.at(1).velocity() == mock_1.velocity);
    CHECK(vec.at(2).velocity() == mock_2.velocity);
    CHECK(vec.at(3).velocity() == mock_3.velocity);
  }
}

TEST_CASE_TEMPLATE("proxy mutation via at", T, layout::aos, layout::soa) {
  container<T> vec {mock_0, mock_1, mock_2, mock_3};

  SUBCASE("modify id") {
    vec.at(0).id() = 99;
    CHECK(vec.at(0).id() == 99);
    CHECK(vec.at(1).id() == mock_1.id);
  }

  SUBCASE("modify density") {
    vec.at(2).density() = -1.5;
    CHECK(vec.at(2).density() == -1.5);
    CHECK(vec.at(0).density() == mock_0.density);
  }

  SUBCASE("modify velocity") {
    vec.at(1).velocity() = {5.0, 6.0, 7.0};
    CHECK(vec.at(1).velocity() == (std::array<std::double_t, 3> {5.0, 6.0, 7.0}));
    CHECK(vec.at(0).velocity() == mock_0.velocity);
  }
}

TEST_CASE_TEMPLATE("proxy mutation via operator[]", T, layout::aos, layout::soa) {
  container<T> vec {mock_0, mock_1, mock_2};

  vec[0].id()      = 77;
  vec[0].density() = 3.14;

  CHECK(vec[0].id() == 77);
  CHECK(vec[0].density() == 3.14);
  CHECK(vec[1].id() == mock_1.id);
}

TEST_CASE_TEMPLATE("proxy assignment", T, layout::aos, layout::soa) {
  container<T> vec {mock_0, mock_1, mock_2};

  vec.at(0) = mock_3;

  CHECK(vec.at(0) == mock_3);
  CHECK(vec.at(1) == mock_1);
  CHECK(vec.at(2) == mock_2);
}

TEST_CASE_TEMPLATE("Const access", T, layout::aos, layout::soa) {
  container<T> const vec {mock_0, mock_1, mock_2};

  CHECK(vec.at(0) == mock_0);
  CHECK(vec.at(1) == mock_1);
  CHECK(vec[2] == mock_2);
}

// *** Comparison ***

TEST_CASE_TEMPLATE("Comparison", T, layout::aos, layout::soa) {
  container<T> vec1 {mock_0, mock_1, mock_2, mock_3};
  container<T> vec2 {mock_0, mock_1, mock_2, mock_3};
  container<T> vec3 {mock_0, mock_1, mock_2};

  CHECK(vec1 == vec2);
  CHECK(vec1 != vec3);
}

TEST_CASE_TEMPLATE("Comparison after mutation", T, layout::aos, layout::soa) {
  container<T> vec1 {mock_0, mock_1, mock_2};
  container<T> vec2 {mock_0, mock_1, mock_2};

  CHECK(vec1 == vec2);

  vec1.at(0).id() = 99;

  CHECK(vec1 != vec2);
}

// *** Modifiers ***

TEST_CASE_TEMPLATE("push_back value", T, layout::aos, layout::soa) {
  container<T> vec {mock_0, mock_1, mock_2};
  container<T> vec2;
  vec2.push_back(mock_0);
  vec2.push_back(mock_1);
  vec2.push_back(mock_2);

  CHECK(vec == vec2);
  vec2.push_back(mock_2);
  CHECK(vec != vec2);

  auto mock_view = vec2.back();
  vec.push_back(mock_view);
  CHECK(vec == vec2);
}

TEST_CASE_TEMPLATE("push_back proxy view", T, layout::aos, layout::soa) {
  container<T> source {mock_0, mock_1, mock_2, mock_3};
  container<T> dest;

  for (auto elem: source) {
    dest.push_back(elem);
  }

  CHECK(dest == source);
}

TEST_CASE_TEMPLATE("push_back increases size", T, layout::aos, layout::soa) {
  container<T> vec;

  CHECK(vec.size() == 0U);
  vec.push_back(mock_0);
  CHECK(vec.size() == 1U);
  vec.push_back(mock_1);
  CHECK(vec.size() == 2U);
  vec.push_back(mock_2);
  CHECK(vec.size() == 3U);
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

TEST_CASE_TEMPLATE("pop_back decreases size", T, layout::aos, layout::soa) {
  container<T> vec {mock_0, mock_1, mock_2, mock_3};

  CHECK(vec.size() == 4U);
  vec.pop_back();
  CHECK(vec.size() == 3U);
  vec.pop_back();
  CHECK(vec.size() == 2U);
}

TEST_CASE_TEMPLATE("pop_back then push_back", T, layout::aos, layout::soa) {
  container<T> vec {mock_0, mock_1, mock_2};
  container<T> expected {mock_0, mock_1, mock_3};

  vec.pop_back();
  vec.push_back(mock_3);

  CHECK(vec == expected);
}

TEST_CASE_TEMPLATE("erase last element", T, layout::aos, layout::soa) {
  container<T> vec1 {mock_0, mock_1, mock_2, mock_3};
  container<T> vec2 {mock_0, mock_1, mock_2, mock_3};
  container<T> vec3 {mock_0, mock_1, mock_2};

  auto it = vec1.erase(vec1.begin() + 3);
  CHECK(it == vec1.end());
  CHECK(vec1 == vec3);
  CHECK(vec1 != vec2);

  vec2.erase(vec3.begin() + 3);
  CHECK(vec1 == vec2);
}

TEST_CASE_TEMPLATE("erase first element", T, layout::aos, layout::soa) {
  container<T> vec {mock_0, mock_1, mock_2, mock_3};
  container<T> expected {mock_1, mock_2, mock_3};

  vec.erase(vec.begin());

  CHECK(vec == expected);
  CHECK(vec.size() == 3U);
}

TEST_CASE_TEMPLATE("erase middle element", T, layout::aos, layout::soa) {
  container<T> vec {mock_0, mock_1, mock_2, mock_3};
  container<T> expected {mock_0, mock_2, mock_3};

  vec.erase(vec.begin() + 1);

  CHECK(vec == expected);
  CHECK(vec.size() == 3U);
}

TEST_CASE_TEMPLATE("erase returns iterator to next", T, layout::aos, layout::soa) {
  container<T> vec {mock_0, mock_1, mock_2, mock_3};

  auto it     = vec.erase(vec.begin() + 1);
  auto it_end = vec.end();

  CHECK(it != it_end);
  CHECK((*it).id() == mock_2.id);
}

// *** Iterators ***

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

TEST_CASE_TEMPLATE("Const iterator", T, layout::aos, layout::soa) {
  container<T> vec {mock_0, mock_1, mock_2, mock_3};

  std::int32_t i  = 0;
  std::double_t j = 12.15;

  for (auto it = vec.cbegin(); it != vec.cend(); ++it) {
    CHECK((*it).id() == i++);
    CHECK((*it).density() == j++);
  }
}

TEST_CASE_TEMPLATE("Const vector iterator", T, layout::aos, layout::soa) {
  container<T> const vec {mock_0, mock_1, mock_2, mock_3};

  std::int32_t i = 0;
  for (auto it = vec.begin(); it != vec.end(); ++it) {
    CHECK((*it).id() == i++);
  }
}

TEST_CASE_TEMPLATE("Iterator post-increment", T, layout::aos, layout::soa) {
  container<T> vec {mock_0, mock_1, mock_2};

  auto it  = vec.begin();
  auto old = it++;
  CHECK((*old).id() == mock_0.id);
  CHECK((*it).id() == mock_1.id);
}

TEST_CASE_TEMPLATE("Iterator arithmetic", T, layout::aos, layout::soa) {
  container<T> vec {mock_0, mock_1, mock_2, mock_3};

  auto begin = vec.begin();
  auto end   = vec.end();

  SUBCASE("operator+") {
    auto it = begin + 2;
    CHECK((*it).id() == mock_2.id);
  }

  SUBCASE("operator-") {
    auto it = end - 1;
    CHECK((*it).id() == mock_3.id);
  }

  SUBCASE("difference") {
    CHECK(end - begin == 4);
    CHECK(begin - begin == 0);
    CHECK((begin + 3) - begin == 3);
  }
}

TEST_CASE_TEMPLATE("Iterator equality", T, layout::aos, layout::soa) {
  container<T> vec {mock_0, mock_1, mock_2};

  CHECK(vec.begin() == vec.begin());
  CHECK(vec.end() == vec.end());
  CHECK(vec.begin() != vec.end());
  CHECK(vec.begin() + 3 == vec.end());
}

// *** Range iteration ***

TEST_CASE_TEMPLATE("Range iteration", T, layout::aos, layout::soa) {
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

TEST_CASE_TEMPLATE("Range iteration reads velocity", T, layout::aos, layout::soa) {
  container<T> vec {mock_0, mock_1, mock_2, mock_3};
  std::array<Mock const*, 4> const mocks {&mock_0, &mock_1, &mock_2, &mock_3};

  std::size_t idx = 0;
  for (auto elem: vec) {
    CHECK(elem.velocity() == mocks[idx]->velocity);
    ++idx;
  }
}

TEST_CASE_TEMPLATE("Range iteration mutates elements", T, layout::aos, layout::soa) {
  container<T> vec {mock_0, mock_1, mock_2, mock_3};

  for (auto elem: vec) {
    elem.id() = -1;
  }

  for (auto elem: vec) {
    CHECK(elem.id() == -1);
  }
}

// *** Algorithm compatibility ***

TEST_CASE_TEMPLATE("std::find_if compatibility", T, layout::aos, layout::soa) {
  container<T> vec {mock_0, mock_1, mock_2, mock_3};

  auto it = std::find_if(vec.begin(), vec.end(), [](auto const& elem) { return elem.id() == 2; });
  CHECK(it != vec.end());
  CHECK((*it).id() == mock_2.id);
}

TEST_CASE_TEMPLATE("std::count_if compatibility", T, layout::aos, layout::soa) {
  container<T> vec {mock_0, mock_1, mock_2, mock_3};

  auto count = std::count_if(vec.begin(), vec.end(), [](auto const& elem) { return elem.density() > 13.0; });
  CHECK(count == 3);
}

TEST_SUITE_END();
