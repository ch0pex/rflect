/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file main.cpp
 * @version 1.0
 * @date 23/03/2025
 * @brief Short description
 *
 * Longer description
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "common.hpp"

DOCTEST_TEST_SUITE_BEGIN("Integration tests");

TEST_CASE("Large file") {
  for (sim::i8 i = 1; i <= 5; ++i)
    test_simulation("large", i);
}

TEST_CASE("Small file") {
  for (sim::i8 i = 1; i <= 5; ++i)
    test_simulation("small", i);
}

DOCTEST_TEST_SUITE_END();