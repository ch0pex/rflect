/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file test_multi_array.cpp
 * @version 1.0
 * @date 06/04/2026
 * @brief Comprehensive tests for multi_array
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "test_containers.hpp"

using namespace rflect;

TEST_SUITE_BEGIN("Multi Array");

// *** Constructors ***

TEST_CASE("Default constructor") {
  multi_array<Mock, 4> arr;
  CHECK(arr.size() == 4U);
  CHECK(arr.max_size() == 4U);
  CHECK(arr.empty() == false);
}

TEST_CASE("Zero-size array is empty") {
  multi_array<Mock, 0> arr;
  CHECK(arr.size() == 0U);
  CHECK(arr.empty() == true);
}

TEST_CASE("Initializer list constructor") {
  multi_array<Mock, 4> arr {mock_0, mock_1, mock_2, mock_3};
  CHECK(arr.size() == 4U);
}

// *** Capacity ***

TEST_CASE("Capacity of fixed-size array") {
  multi_array<Mock, 10> arr;
  CHECK(arr.size() == 10U);
  CHECK(arr.max_size() == 10U);
  CHECK(arr.empty() == false);

  multi_array<Mock, 0> empty_arr;
  CHECK(empty_arr.size() == 0U);
  CHECK(empty_arr.empty() == true);
}

TEST_CASE("size equals max_size") {
  multi_array<Mock, 7> arr;
  CHECK(arr.size() == arr.max_size());
}

// *** SoA structure via items<N>() ***

TEST_CASE("items<0> gives id column") {
  multi_array<Mock, 4> arr {mock_0, mock_1, mock_2, mock_3};

  auto& id_col = arr.items<0>();
  CHECK(id_col[0] == mock_0.id);
  CHECK(id_col[1] == mock_1.id);
  CHECK(id_col[2] == mock_2.id);
  CHECK(id_col[3] == mock_3.id);
}

TEST_CASE("items<1> gives density column") {
  multi_array<Mock, 4> arr {mock_0, mock_1, mock_2, mock_3};

  auto& density_col = arr.items<1>();
  CHECK(density_col[0] == mock_0.density);
  CHECK(density_col[1] == mock_1.density);
  CHECK(density_col[2] == mock_2.density);
  CHECK(density_col[3] == mock_3.density);
}

TEST_CASE("items<2> gives velocity column") {
  multi_array<Mock, 4> arr {mock_0, mock_1, mock_2, mock_3};

  auto& velocity_col = arr.items<2>();
  CHECK(velocity_col[0] == mock_0.velocity);
  CHECK(velocity_col[1] == mock_1.velocity);
  CHECK(velocity_col[2] == mock_2.velocity);
  CHECK(velocity_col[3] == mock_3.velocity);
}

TEST_CASE("items<N> returns writable reference") {
  multi_array<Mock, 4> arr {mock_0, mock_1, mock_2, mock_3};

  arr.items<0>()[0] = 99;
  arr.items<1>()[1] = 0.0;
  arr.items<2>()[2] = {9.0, 8.0, 7.0};

  CHECK(arr.items<0>()[0] == 99);
  CHECK(arr.items<1>()[1] == 0.0);
  CHECK(arr.items<2>()[2] == (std::array<std::double_t, 3>{9.0, 8.0, 7.0}));

  // Verify other elements are untouched
  CHECK(arr.items<0>()[1] == mock_1.id);
  CHECK(arr.items<1>()[0] == mock_0.density);
  CHECK(arr.items<2>()[3] == mock_3.velocity);
}

TEST_CASE("items<N> on const array") {
  multi_array<Mock, 4> const arr {mock_0, mock_1, mock_2, mock_3};

  auto const& id_col      = arr.items<0>();
  auto const& density_col = arr.items<1>();

  CHECK(id_col[0] == mock_0.id);
  CHECK(density_col[2] == mock_2.density);
}

TEST_CASE("SoA columns have correct size") {
  multi_array<Mock, 8> arr;

  CHECK(arr.items<0>().size() == 8U);
  CHECK(arr.items<1>().size() == 8U);
  CHECK(arr.items<2>().size() == 8U);
}

// *** items<name>() by field name ***

TEST_CASE("items<name> gives named column") {
  multi_array<Mock, 4> arr {mock_0, mock_1, mock_2, mock_3};

  constexpr auto id_field       = std::define_static_string("id");
  constexpr auto density_field  = std::define_static_string("density");
  constexpr auto velocity_field = std::define_static_string("velocity");

  auto& id_col       = arr.items<id_field>();
  auto& density_col  = arr.items<density_field>();
  auto& velocity_col = arr.items<velocity_field>();

  CHECK(id_col[0] == mock_0.id);
  CHECK(density_col[1] == mock_1.density);
  CHECK(velocity_col[2] == mock_2.velocity);
}

TEST_CASE("items<name> and items<N> return same column") {
  multi_array<Mock, 4> arr {mock_0, mock_1, mock_2, mock_3};

  constexpr auto id_field = std::define_static_string("id");

  auto& by_index = arr.items<0>();
  auto& by_name  = arr.items<id_field>();

  for (std::size_t i = 0; i < arr.size(); ++i) {
    CHECK(by_index[i] == by_name[i]);
  }
}

// *** front() ***

TEST_CASE("front returns first element") {
  multi_array<Mock, 4> arr {mock_0, mock_1, mock_2, mock_3};

  auto [id, density, velocity] = arr.front();
  CHECK(id == mock_0.id);
  CHECK(density == mock_0.density);
  CHECK(velocity == mock_0.velocity);
}

// *** at() and operator[] ***

TEST_CASE("at returns correct elements") {
  multi_array<Mock, 4> arr {mock_0, mock_1, mock_2, mock_3};

  SUBCASE("index 0") {
    auto [id, density, velocity] = arr.at(0);
    CHECK(id == mock_0.id);
    CHECK(density == mock_0.density);
    CHECK(velocity == mock_0.velocity);
  }

  SUBCASE("index 1") {
    auto [id, density, velocity] = arr.at(1);
    CHECK(id == mock_1.id);
    CHECK(density == mock_1.density);
    CHECK(velocity == mock_1.velocity);
  }

  SUBCASE("index 3") {
    auto [id, density, velocity] = arr.at(3);
    CHECK(id == mock_3.id);
    CHECK(density == mock_3.density);
    CHECK(velocity == mock_3.velocity);
  }
}

TEST_CASE("operator[] returns correct elements") {
  multi_array<Mock, 4> arr {mock_0, mock_1, mock_2, mock_3};

  auto [id0, density0, velocity0] = arr[0];
  auto [id2, density2, velocity2] = arr[2];

  CHECK(id0 == mock_0.id);
  CHECK(density2 == mock_2.density);
  CHECK(velocity2 == mock_2.velocity);
}

// *** to_zip() ***

TEST_CASE("to_zip returns iterable zip view") {
  multi_array<Mock, 4> arr {mock_0, mock_1, mock_2, mock_3};

  auto zip = arr.to_zip();

  std::int32_t expected_id = 0;
  for (auto [id, density, velocity]: zip) {
    CHECK(id == expected_id++);
  }
}

// *** Iteration ***

TEST_CASE("begin/end iteration over all elements") {
  multi_array<Mock, 4> arr {mock_0, mock_1, mock_2, mock_3};

  std::int32_t expected_id     = 0;
  std::double_t expected_density = 12.15;

  for (auto it = arr.begin(); it != arr.end(); ++it) {
    auto [id, density, velocity] = *it;
    CHECK(id == expected_id++);
    CHECK(density == expected_density++);
  }
}

TEST_CASE("cbegin/cend iteration") {
  multi_array<Mock, 4> arr {mock_0, mock_1, mock_2, mock_3};

  std::int32_t expected_id = 0;
  for (auto it = arr.cbegin(); it != arr.cend(); ++it) {
    auto [id, density, velocity] = *it;
    CHECK(id == expected_id++);
  }
}

TEST_CASE("Range-for iteration reads all fields") {
  multi_array<Mock, 4> arr {mock_0, mock_1, mock_2, mock_3};

  std::array<Mock const*, 4> const mocks {&mock_0, &mock_1, &mock_2, &mock_3};
  std::size_t idx = 0;

  for (auto [id, density, velocity]: arr) {
    CHECK(id == mocks[idx]->id);
    CHECK(density == mocks[idx]->density);
    CHECK(velocity == mocks[idx]->velocity);
    ++idx;
  }
  CHECK(idx == 4U);
}

TEST_CASE("Range-for mutation persists") {
  multi_array<Mock, 4> arr {mock_0, mock_1, mock_2, mock_3};

  for (auto [id, density, velocity]: arr) {
    id = -1;
  }

  for (auto id: arr.items<0>()) {
    CHECK(id == -1);
  }
}

// *** Consistency: items and iteration see the same data ***

TEST_CASE("items<N> and iteration are consistent") {
  multi_array<Mock, 4> arr {mock_0, mock_1, mock_2, mock_3};

  auto& id_col = arr.items<0>();

  std::size_t idx = 0;
  for (auto [id, density, velocity]: arr) {
    CHECK(id == id_col[idx++]);
  }
}

TEST_CASE("Mutation via items<N> is visible in iteration") {
  multi_array<Mock, 4> arr {mock_0, mock_1, mock_2, mock_3};

  arr.items<0>()[0] = 42;
  arr.items<1>()[0] = 99.9;

  auto [id, density, velocity] = arr.front();
  CHECK(id == 42);
  CHECK(density == 99.9);
}

TEST_SUITE_END();
