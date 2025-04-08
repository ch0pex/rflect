/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file test_proxy.hpp
 * @version 1.0
 * @date 4/7/25
 * @brief Short description
 *
 * Longer description
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "utility.hpp"


using namespace rflect;

TEST_SUITE_BEGIN("Proxy");

TEST_CASE_TEMPLATE("Modifying", T, layout::aos, layout::soa) {
  dual_vector<Mock, T> mock {mock_0, mock_1};

  auto mock_view = mock.at(0);

  SUBCASE("Member by member") {
    CHECK(mock_view.id() == mock_0.id);
    CHECK(mock_view.density() == mock_0.density);
    CHECK(mock_view.velocity() == mock_0.velocity);

    mock_view.id() = 1;
    mock_view.density() += 1;
    mock_view.velocity() = {0.0};

    CHECK(mock_view.id() == mock_1.id);
    CHECK(mock_view.density() == mock_1.density);
    CHECK(mock_view.velocity() == mock_1.velocity);
  }

  SUBCASE("Replace by value") {
    CHECK(mock_view.id() == mock_0.id);
    CHECK(mock_view.density() == mock_0.density);
    CHECK(mock_view.velocity() == mock_0.velocity);

    auto assign = (mock_view = mock_1);

    CHECK(std::same_as<decltype(assign), decltype(mock_view)>);
    CHECK(mock_view.id() == mock_1.id);
    CHECK(mock_view.density() == mock_1.density);
    CHECK(mock_view.velocity() == mock_1.velocity);
  }

  SUBCASE("Replace by view") {
    CHECK(mock_view.id() == mock_0.id);
    CHECK(mock_view.density() == mock_0.density);
    CHECK(mock_view.velocity() == mock_0.velocity);

    auto assign = (mock_view = mock.at(1));

    CHECK(std::same_as<decltype(assign), decltype(mock_view)>);
    CHECK(mock_view.id() == mock_1.id);
    CHECK(mock_view.density() == mock_1.density);
    CHECK(mock_view.velocity() == mock_1.velocity);
  }
}


TEST_SUITE_END();
