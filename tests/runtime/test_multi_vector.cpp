/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file test_multi_vector.cpp
 * @version 1.0
 * @date 06/04/2026
 * @brief Comprehensive tests for multi_vector
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "test_containers.hpp"

using namespace rflect;

TEST_SUITE_BEGIN("Multi Vector");

// *** Constructors ***

TEST_CASE("Default constructor") {
  multi_vector<Mock> vec;
  CHECK(vec.size() == 0U);
  CHECK(vec.empty() == true);
}

TEST_CASE("Initializer list constructor") {
  multi_vector<Mock> vec {mock_0, mock_1, mock_2};
  CHECK(vec.size() == 3U);
  CHECK(vec.empty() == false);
}

TEST_CASE("Explicit size constructor") {
  multi_vector<Mock> vec(4);
  CHECK(vec.size() == 4U);
  CHECK(vec.empty() == false);
}

// *** Capacity ***

TEST_CASE("size after construction") {
  multi_vector<Mock> vec_0;
  multi_vector<Mock> vec_2 {mock_0, mock_1};
  multi_vector<Mock> vec_3 {mock_0, mock_1, mock_2};

  CHECK(vec_0.size() == 0U);
  CHECK(vec_2.size() == 2U);
  CHECK(vec_3.size() == 3U);
}

TEST_CASE("empty") {
  multi_vector<Mock> empty_vec;
  multi_vector<Mock> nonempty_vec {mock_0};

  CHECK(empty_vec.empty() == true);
  CHECK(nonempty_vec.empty() == false);
}

TEST_CASE("capacity grows with push_back") {
  multi_vector<Mock> vec;
  vec.push_back(mock_0);
  vec.push_back(mock_1);
  vec.push_back(mock_2);

  CHECK(vec.size() == 3U);
  CHECK(vec.capacity() >= 3U);
}

// *** SoA structure via items<N>() ***

TEST_CASE("items<0> gives id column") {
  multi_vector<Mock> vec {mock_0, mock_1, mock_2, mock_3};

  auto& id_col = vec.items<0>();
  CHECK(id_col.size() == 4U);
  CHECK(id_col[0] == mock_0.id);
  CHECK(id_col[1] == mock_1.id);
  CHECK(id_col[2] == mock_2.id);
  CHECK(id_col[3] == mock_3.id);
}

TEST_CASE("items<1> gives density column") {
  multi_vector<Mock> vec {mock_0, mock_1, mock_2, mock_3};

  auto& density_col = vec.items<1>();
  CHECK(density_col.size() == 4U);
  CHECK(density_col[0] == mock_0.density);
  CHECK(density_col[1] == mock_1.density);
  CHECK(density_col[2] == mock_2.density);
  CHECK(density_col[3] == mock_3.density);
}

TEST_CASE("items<2> gives velocity column") {
  multi_vector<Mock> vec {mock_0, mock_1, mock_2, mock_3};

  auto& velocity_col = vec.items<2>();
  CHECK(velocity_col.size() == 4U);
  CHECK(velocity_col[0] == mock_0.velocity);
  CHECK(velocity_col[1] == mock_1.velocity);
  CHECK(velocity_col[2] == mock_2.velocity);
  CHECK(velocity_col[3] == mock_3.velocity);
}

TEST_CASE("items<N> returns writable reference") {
  multi_vector<Mock> vec {mock_0, mock_1, mock_2};

  vec.items<0>()[0] = 77;
  vec.items<1>()[1] = 55.5;
  vec.items<2>()[2] = {4.0, 5.0, 6.0};

  CHECK(vec.items<0>()[0] == 77);
  CHECK(vec.items<1>()[1] == 55.5);
  CHECK(vec.items<2>()[2] == (std::array<std::double_t, 3>{4.0, 5.0, 6.0}));

  // Unmodified elements are unchanged
  CHECK(vec.items<0>()[1] == mock_1.id);
  CHECK(vec.items<1>()[0] == mock_0.density);
  CHECK(vec.items<2>()[0] == mock_0.velocity);
}

TEST_CASE("All columns have the same size") {
  multi_vector<Mock> vec {mock_0, mock_1, mock_2};

  CHECK(vec.items<0>().size() == vec.size());
  CHECK(vec.items<1>().size() == vec.size());
  CHECK(vec.items<2>().size() == vec.size());
}

// *** items<name>() by field name ***

TEST_CASE("items<name> gives named column") {
  multi_vector<Mock> vec {mock_0, mock_1, mock_2, mock_3};

  constexpr auto id_field       = std::define_static_string("id");
  constexpr auto density_field  = std::define_static_string("density");
  constexpr auto velocity_field = std::define_static_string("velocity");

  auto& id_col       = vec.items<id_field>();
  auto& density_col  = vec.items<density_field>();
  auto& velocity_col = vec.items<velocity_field>();

  CHECK(id_col[0] == mock_0.id);
  CHECK(density_col[2] == mock_2.density);
  CHECK(velocity_col[3] == mock_3.velocity);
}

TEST_CASE("items<name> and items<N> return same column") {
  multi_vector<Mock> vec {mock_0, mock_1, mock_2};

  constexpr auto id_field = std::define_static_string("id");

  auto& by_index = vec.items<0>();
  auto& by_name  = vec.items<id_field>();

  for (std::size_t i = 0; i < vec.size(); ++i) {
    CHECK(by_index[i] == by_name[i]);
  }
}

// *** front() and back() ***

TEST_CASE("front and back after construction") {
  multi_vector<Mock> vec {mock_0, mock_1, mock_2, mock_3};

  auto [front_id, front_density, front_velocity] = vec.front();
  CHECK(front_id == mock_0.id);
  CHECK(front_density == mock_0.density);
  CHECK(front_velocity == mock_0.velocity);

  auto [back_id, back_density, back_velocity] = vec.back();
  CHECK(back_id == mock_3.id);
  CHECK(back_density == mock_3.density);
  CHECK(back_velocity == mock_3.velocity);
}

TEST_CASE("front equals back on single-element vector") {
  multi_vector<Mock> vec {mock_2};

  auto [fid, fdensity, fvelocity] = vec.front();
  auto [bid, bdensity, bvelocity] = vec.back();

  CHECK(fid == bid);
  CHECK(fdensity == bdensity);
  CHECK(fvelocity == bvelocity);
}

// *** Modifiers: push_back ***

TEST_CASE("push_back value type adds element") {
  multi_vector<Mock> vec;

  vec.push_back(mock_0);
  CHECK(vec.size() == 1U);
  CHECK(vec.items<0>()[0] == mock_0.id);
  CHECK(vec.items<1>()[0] == mock_0.density);
  CHECK(vec.items<2>()[0] == mock_0.velocity);

  vec.push_back(mock_1);
  CHECK(vec.size() == 2U);
  CHECK(vec.items<0>()[1] == mock_1.id);
}

TEST_CASE("push_back adds to all columns simultaneously") {
  multi_vector<Mock> vec;

  vec.push_back(mock_3);

  CHECK(vec.items<0>().size() == 1U);
  CHECK(vec.items<1>().size() == 1U);
  CHECK(vec.items<2>().size() == 1U);

  CHECK(vec.items<0>()[0] == mock_3.id);
  CHECK(vec.items<1>()[0] == mock_3.density);
  CHECK(vec.items<2>()[0] == mock_3.velocity);
}

TEST_CASE("push_back produces same result as initializer list") {
  multi_vector<Mock> from_init {mock_0, mock_1, mock_2};

  multi_vector<Mock> from_push;
  from_push.push_back(mock_0);
  from_push.push_back(mock_1);
  from_push.push_back(mock_2);

  CHECK(from_init.size() == from_push.size());
  for (std::size_t i = 0; i < from_init.size(); ++i) {
    CHECK(from_init.items<0>()[i] == from_push.items<0>()[i]);
    CHECK(from_init.items<1>()[i] == from_push.items<1>()[i]);
    CHECK(from_init.items<2>()[i] == from_push.items<2>()[i]);
  }
}

// *** Modifiers: pop_back ***

TEST_CASE("pop_back removes last element") {
  multi_vector<Mock> vec {mock_0, mock_1, mock_2, mock_3};

  vec.pop_back();

  CHECK(vec.size() == 3U);
  CHECK(vec.items<0>()[2] == mock_2.id);
  CHECK(vec.items<1>()[2] == mock_2.density);
}

TEST_CASE("pop_back removes from all columns") {
  multi_vector<Mock> vec {mock_0, mock_1, mock_2};

  vec.pop_back();

  CHECK(vec.items<0>().size() == 2U);
  CHECK(vec.items<1>().size() == 2U);
  CHECK(vec.items<2>().size() == 2U);
}

TEST_CASE("pop_back then push_back") {
  multi_vector<Mock> vec {mock_0, mock_1, mock_2};

  vec.pop_back();
  vec.push_back(mock_3);

  CHECK(vec.size() == 3U);
  CHECK(vec.items<0>()[2] == mock_3.id);
  CHECK(vec.items<1>()[2] == mock_3.density);
  CHECK(vec.items<2>()[2] == mock_3.velocity);
}

TEST_CASE("Multiple pop_back") {
  multi_vector<Mock> vec {mock_0, mock_1, mock_2, mock_3};

  vec.pop_back();
  vec.pop_back();

  CHECK(vec.size() == 2U);
  CHECK(vec.items<0>()[0] == mock_0.id);
  CHECK(vec.items<0>()[1] == mock_1.id);
}

// *** Modifiers: erase ***

TEST_CASE("erase last element") {
  multi_vector<Mock> vec {mock_0, mock_1, mock_2, mock_3};

  auto it = vec.erase(vec.begin() + 3);
  CHECK(it == vec.end());
  CHECK(vec.size() == 3U);
  CHECK(vec.items<0>()[2] == mock_2.id);
}

TEST_CASE("erase first element") {
  multi_vector<Mock> vec {mock_0, mock_1, mock_2, mock_3};

  vec.erase(vec.begin());

  CHECK(vec.size() == 3U);
  CHECK(vec.items<0>()[0] == mock_1.id);
  CHECK(vec.items<1>()[0] == mock_1.density);
  CHECK(vec.items<2>()[0] == mock_1.velocity);
}

TEST_CASE("erase middle element") {
  multi_vector<Mock> vec {mock_0, mock_1, mock_2, mock_3};

  vec.erase(vec.begin() + 1);

  CHECK(vec.size() == 3U);
  CHECK(vec.items<0>()[0] == mock_0.id);
  CHECK(vec.items<0>()[1] == mock_2.id);
  CHECK(vec.items<0>()[2] == mock_3.id);
}

TEST_CASE("erase removes from all columns") {
  multi_vector<Mock> vec {mock_0, mock_1, mock_2};

  vec.erase(vec.begin() + 1);

  CHECK(vec.items<0>().size() == 2U);
  CHECK(vec.items<1>().size() == 2U);
  CHECK(vec.items<2>().size() == 2U);

  CHECK(vec.items<0>()[0] == mock_0.id);
  CHECK(vec.items<0>()[1] == mock_2.id);
  CHECK(vec.items<1>()[0] == mock_0.density);
  CHECK(vec.items<1>()[1] == mock_2.density);
  CHECK(vec.items<2>()[0] == mock_0.velocity);
  CHECK(vec.items<2>()[1] == mock_2.velocity);
}

TEST_CASE("erase range") {
  multi_vector<Mock> vec {mock_0, mock_1, mock_2, mock_3};

  vec.erase(vec.begin() + 1, vec.begin() + 3);

  CHECK(vec.size() == 2U);
  CHECK(vec.items<0>()[0] == mock_0.id);
  CHECK(vec.items<0>()[1] == mock_3.id);
}

TEST_CASE("erase range removes from all columns") {
  multi_vector<Mock> vec {mock_0, mock_1, mock_2, mock_3};

  vec.erase(vec.begin(), vec.begin() + 2);

  CHECK(vec.items<0>().size() == 2U);
  CHECK(vec.items<1>().size() == 2U);
  CHECK(vec.items<2>().size() == 2U);

  CHECK(vec.items<0>()[0] == mock_2.id);
  CHECK(vec.items<0>()[1] == mock_3.id);
}

TEST_CASE("erase entire range leaves empty vector") {
  multi_vector<Mock> vec {mock_0, mock_1, mock_2};

  vec.erase(vec.begin(), vec.end());

  CHECK(vec.size() == 0U);
  CHECK(vec.empty() == true);
}

// *** Iteration ***

TEST_CASE("begin/end iteration reads all fields") {
  multi_vector<Mock> vec {mock_0, mock_1, mock_2, mock_3};

  std::int32_t expected_id     = 0;
  std::double_t expected_density = 12.15;

  for (auto it = vec.begin(); it != vec.end(); ++it) {
    auto [id, density, velocity] = *it;
    CHECK(id == expected_id++);
    CHECK(density == expected_density++);
  }
}

TEST_CASE("cbegin/cend iteration") {
  multi_vector<Mock> vec {mock_0, mock_1, mock_2, mock_3};

  std::int32_t expected_id = 0;
  for (auto it = vec.cbegin(); it != vec.cend(); ++it) {
    auto [id, density, velocity] = *it;
    CHECK(id == expected_id++);
  }
}

TEST_CASE("Range-for reads all fields") {
  multi_vector<Mock> vec {mock_0, mock_1, mock_2, mock_3};

  std::array<Mock const*, 4> const mocks {&mock_0, &mock_1, &mock_2, &mock_3};
  std::size_t idx = 0;

  for (auto [id, density, velocity]: vec) {
    CHECK(id == mocks[idx]->id);
    CHECK(density == mocks[idx]->density);
    CHECK(velocity == mocks[idx]->velocity);
    ++idx;
  }
  CHECK(idx == 4U);
}

TEST_CASE("Range-for mutation persists") {
  multi_vector<Mock> vec {mock_0, mock_1, mock_2};

  for (auto [id, density, velocity]: vec) {
    id = -1;
  }

  for (std::size_t i = 0; i < vec.size(); ++i) {
    CHECK(vec.items<0>()[i] == -1);
  }
}

// *** to_zip() ***

TEST_CASE("to_zip returns iterable view") {
  multi_vector<Mock> vec {mock_0, mock_1, mock_2, mock_3};

  auto zip            = vec.to_zip();
  std::int32_t exp_id = 0;
  for (auto [id, density, velocity]: zip) {
    CHECK(id == exp_id++);
  }
  CHECK(exp_id == 4);
}

// *** Consistency: items and iteration see the same data ***

TEST_CASE("items<N> and iteration are consistent") {
  multi_vector<Mock> vec {mock_0, mock_1, mock_2, mock_3};

  auto& id_col      = vec.items<0>();
  auto& density_col = vec.items<1>();

  std::size_t idx = 0;
  for (auto [id, density, velocity]: vec) {
    CHECK(id == id_col[idx]);
    CHECK(density == density_col[idx]);
    ++idx;
  }
}

TEST_CASE("Mutation via items<N> visible in iteration") {
  multi_vector<Mock> vec {mock_0, mock_1, mock_2};

  vec.items<0>()[0] = 100;
  vec.items<1>()[0] = 200.0;
  vec.items<2>()[0] = {7.0, 8.0, 9.0};

  auto [id, density, velocity] = vec.front();
  CHECK(id == 100);
  CHECK(density == 200.0);
  CHECK(velocity == (std::array<std::double_t, 3>{7.0, 8.0, 9.0}));
}

TEST_CASE("push_back visible in items<N>") {
  multi_vector<Mock> vec {mock_0, mock_1};

  vec.push_back(mock_2);

  CHECK(vec.items<0>()[2] == mock_2.id);
  CHECK(vec.items<1>()[2] == mock_2.density);
  CHECK(vec.items<2>()[2] == mock_2.velocity);
}

TEST_CASE("pop_back visible in items<N>") {
  multi_vector<Mock> vec {mock_0, mock_1, mock_2};

  vec.pop_back();

  CHECK(vec.items<0>().size() == 2U);
  CHECK(vec.items<0>().back() == mock_1.id);
}

TEST_SUITE_END();
