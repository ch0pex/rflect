/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file test_dual_array.cpp
 * @version 1.0
 * @date 28/03/2025
 * @brief Comprehensive tests for dual_array
 */

#include <array>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "test_containers.hpp"

using namespace rflect;

constexpr std::size_t array_size = 50U;

template<typename Layout>
using container = dual_array<Mock, array_size, Layout>;

TEST_SUITE_BEGIN("Dual Array");

// *** Constructors ***

TEST_CASE_TEMPLATE("Default constructor", T, layout::aos, layout::soa) {
  container<T> arr;
  CHECK(arr.size() == array_size);
}

TEST_CASE_TEMPLATE("Initializer list constructor", T, layout::aos, layout::soa) {
  container<T> arr {mock_0, mock_1, mock_2};
  CHECK(arr.size() == array_size);
}

TEST_CASE_TEMPLATE("Full initializer list", T, layout::aos, layout::soa) {
  dual_array<Mock, 4, T> arr {mock_0, mock_1, mock_2, mock_3};
  CHECK(arr.size() == 4U);
  CHECK(arr[0] == mock_0);
  CHECK(arr[1] == mock_1);
  CHECK(arr[2] == mock_2);
  CHECK(arr[3] == mock_3);
}

// *** Element access ***

TEST_CASE_TEMPLATE("Element access", T, layout::aos, layout::soa) { test_element_access<container<T>>(); }

TEST_CASE_TEMPLATE("front", T, layout::aos, layout::soa) {
  dual_array<Mock, 4, T> arr {mock_0, mock_1, mock_2, mock_3};

  CHECK(arr.front() == mock_0);
  CHECK(arr.front() == arr.at(0));
}

TEST_CASE_TEMPLATE("back", T, layout::aos, layout::soa) {
  dual_array<Mock, 4, T> arr {mock_0, mock_1, mock_2, mock_3};

  CHECK(arr.back() == mock_3);
  CHECK(arr.back() == arr.at(3));
}

TEST_CASE_TEMPLATE("front equals back on single-element array", T, layout::aos, layout::soa) {
  dual_array<Mock, 1, T> arr {mock_2};

  CHECK(arr.front() == mock_2);
  CHECK(arr.back() == mock_2);
  CHECK(arr.front() == arr.back());
}

TEST_CASE_TEMPLATE("at returns proxy that reads all fields", T, layout::aos, layout::soa) {
  dual_array<Mock, 4, T> arr {mock_0, mock_1, mock_2, mock_3};

  SUBCASE("id field") {
    CHECK(arr.at(0).id() == mock_0.id);
    CHECK(arr.at(1).id() == mock_1.id);
    CHECK(arr.at(2).id() == mock_2.id);
    CHECK(arr.at(3).id() == mock_3.id);
  }

  SUBCASE("density field") {
    CHECK(arr.at(0).density() == mock_0.density);
    CHECK(arr.at(1).density() == mock_1.density);
    CHECK(arr.at(2).density() == mock_2.density);
    CHECK(arr.at(3).density() == mock_3.density);
  }

  SUBCASE("velocity field") {
    CHECK(arr.at(0).velocity() == mock_0.velocity);
    CHECK(arr.at(1).velocity() == mock_1.velocity);
    CHECK(arr.at(2).velocity() == mock_2.velocity);
    CHECK(arr.at(3).velocity() == mock_3.velocity);
  }
}

TEST_CASE_TEMPLATE("proxy mutation via at", T, layout::aos, layout::soa) {
  dual_array<Mock, 4, T> arr {mock_0, mock_1, mock_2, mock_3};

  SUBCASE("modify id") {
    arr.at(0).id() = 99;
    CHECK(arr.at(0).id() == 99);
    CHECK(arr.at(1).id() == mock_1.id);
  }

  SUBCASE("modify density") {
    arr.at(1).density() = 99.99;
    CHECK(arr.at(1).density() == 99.99);
    CHECK(arr.at(0).density() == mock_0.density);
  }

  SUBCASE("modify velocity") {
    arr.at(2).velocity() = {9.0, 8.0, 7.0};
    CHECK(arr.at(2).velocity() == (std::array<std::double_t, 3> {9.0, 8.0, 7.0}));
    CHECK(arr.at(0).velocity() == mock_0.velocity);
  }
}

TEST_CASE_TEMPLATE("proxy mutation via operator[]", T, layout::aos, layout::soa) {
  dual_array<Mock, 4, T> arr {mock_0, mock_1, mock_2, mock_3};

  arr[0].id()      = 42;
  arr[0].density() = 0.5;

  CHECK(arr[0].id() == 42);
  CHECK(arr[0].density() == 0.5);
  CHECK(arr[1].id() == mock_1.id);
}

TEST_CASE_TEMPLATE("proxy assignment via at", T, layout::aos, layout::soa) {
  dual_array<Mock, 4, T> arr {mock_0, mock_1, mock_2, mock_3};

  arr.at(0) = mock_3;

  CHECK(arr.at(0) == mock_3);
  CHECK(arr.at(1) == mock_1);
}

// *** Capacity ***

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

TEST_CASE_TEMPLATE("size equals max_size for fixed array", T, layout::aos, layout::soa) {
  dual_array<Mock, 10, T> arr;

  CHECK(arr.size() == arr.max_size());
  CHECK(arr.size() == 10U);
}

// *** Iterators ***

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

  SUBCASE("const container begin - end") {
    dual_array<Mock, 4, T> const cmock {mock_0, mock_1, mock_2, mock_3};
    for (auto it = cmock.begin(); it != cmock.end(); ++it) {
      auto id = (*it).id();
      CHECK(id == i++);
    }
  }
}

TEST_CASE_TEMPLATE("Iterator post-increment", T, layout::aos, layout::soa) {
  dual_array<Mock, 4, T> arr {mock_0, mock_1, mock_2, mock_3};

  auto it  = arr.begin();
  auto old = it++;
  CHECK((*old).id() == mock_0.id);
  CHECK((*it).id() == mock_1.id);
}

TEST_CASE_TEMPLATE("Iterator arithmetic", T, layout::aos, layout::soa) {
  dual_array<Mock, 4, T> arr {mock_0, mock_1, mock_2, mock_3};

  auto begin = arr.begin();
  auto end   = arr.end();

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
  dual_array<Mock, 4, T> arr {mock_0, mock_1, mock_2, mock_3};

  CHECK(arr.begin() == arr.begin());
  CHECK(arr.end() == arr.end());
  CHECK(arr.begin() != arr.end());
  CHECK(arr.begin() + 4 == arr.end());
}

TEST_CASE_TEMPLATE("Iterator velocity field", T, layout::aos, layout::soa) {
  dual_array<Mock, 4, T> arr {mock_0, mock_1, mock_2, mock_3};
  std::size_t idx = 0;

  for (auto it = arr.begin(); it != arr.end(); ++it, ++idx) {
    CHECK((*it).velocity()[0] == arr.at(idx).velocity()[0]);
    CHECK((*it).velocity()[1] == arr.at(idx).velocity()[1]);
    CHECK((*it).velocity()[2] == arr.at(idx).velocity()[2]);
  }
}

// *** Range iteration ***

TEST_CASE_TEMPLATE("Range iteration", T, layout::aos, layout::soa) {
  static constexpr std::size_t size = 4;
  dual_array<Mock, size, T> mock {mock_0, mock_1, mock_2, mock_3};

  std::int32_t i  = 0;
  std::double_t j = 12.15;

  CHECK(mock.size() == size);

  for (auto elem: mock) {
    CHECK(elem.id() == i++);
    CHECK(elem.density() == j++);
  }
}

TEST_CASE_TEMPLATE("Range iteration reads velocity", T, layout::aos, layout::soa) {
  dual_array<Mock, 4, T> arr {mock_0, mock_1, mock_2, mock_3};
  std::array<Mock const*, 4> const mocks {&mock_0, &mock_1, &mock_2, &mock_3};

  std::size_t idx = 0;
  for (auto elem: arr) {
    CHECK(elem.velocity() == mocks[idx]->velocity);
    ++idx;
  }
}

TEST_CASE_TEMPLATE("Range iteration mutates elements", T, layout::aos, layout::soa) {
  dual_array<Mock, 4, T> arr {mock_0, mock_1, mock_2, mock_3};

  for (auto elem: arr) {
    elem.id() = -1;
  }

  for (auto elem: arr) {
    CHECK(elem.id() == -1);
  }
}

// *** Const correctness ***

TEST_CASE_TEMPLATE("Const access", T, layout::aos, layout::soa) {
  dual_array<Mock, 4, T> const arr {mock_0, mock_1, mock_2, mock_3};

  CHECK(arr.at(0) == mock_0);
  CHECK(arr.at(1) == mock_1);
  CHECK(arr[2] == mock_2);
  CHECK(arr[3] == mock_3);

  CHECK(arr.front() == mock_0);
  CHECK(arr.back() == mock_3);
}

TEST_CASE_TEMPLATE("Const iterator reads all fields", T, layout::aos, layout::soa) {
  dual_array<Mock, 4, T> const arr {mock_0, mock_1, mock_2, mock_3};

  std::int32_t expected_id = 0;
  for (auto it = arr.cbegin(); it != arr.cend(); ++it) {
    CHECK((*it).id() == expected_id++);
    CHECK((*it).density() == doctest::Approx(12.15 + (expected_id - 1)));
  }
}

// *** Algorithm compatibility ***

TEST_CASE_TEMPLATE("std::find_if compatibility", T, layout::aos, layout::soa) {
  dual_array<Mock, 4, T> arr {mock_0, mock_1, mock_2, mock_3};

  auto it = std::find_if(arr.begin(), arr.end(), [](auto const& elem) { return elem.id() == 2; });
  CHECK(it != arr.end());
  CHECK((*it).id() == mock_2.id);
  CHECK((*it).density() == mock_2.density);
}

TEST_CASE_TEMPLATE("std::count_if compatibility", T, layout::aos, layout::soa) {
  dual_array<Mock, 4, T> arr {mock_0, mock_1, mock_2, mock_3};

  auto count = std::count_if(arr.begin(), arr.end(), [](auto const& elem) { return elem.id() >= 2; });
  CHECK(count == 2);
}

TEST_SUITE_END();
