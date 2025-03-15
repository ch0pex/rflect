/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file math.cpp
 * @version 1.0
 * @date 12/03/2025
 * @brief Short description
 *
 * Longer description
 */


#include "math/math.hpp"

#include <iostream>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

using namespace sim;

TEST_CASE("Vector Constructors") {
  {
    math::vec2 vec2 {1.0f, 0.0f};
    CHECK(vec2->x == 1.0f);
    CHECK(vec2->y == 0.0f);

    vec2->x = 2.f;
    vec2->y = 1.f;
    CHECK(vec2->x == 2.0f);
    CHECK(vec2->y == 1.0f);
  }
  {
    math::vec3 vec3 {1.0f, 0.0f, 5.0f};
    CHECK(vec3->x == 1.0f);
    CHECK(vec3->y == 0.0f);
    CHECK(vec3->z == 5.0f);

    vec3->x = 2.f;
    vec3->y = 1.f;
    vec3->z = 5.f;
    CHECK(vec3->x == 2.0f);
    CHECK(vec3->y == 1.0f);
    CHECK(vec3->z == 5.0f);
  }

  {
    math::vec4 vec4 {1.0f, 0.0f, 5.0f, -7.0f};
    CHECK(vec4->x == 1.0f);
    CHECK(vec4->y == 0.0f);
    CHECK(vec4->z == 5.0f);
    CHECK(vec4->w == -7.0f);

    vec4->x = 2.f;
    vec4->y = 1.f;
    vec4->z = 5.f;
    vec4->w = 2.f;
    CHECK(vec4->x == 2.0f);
    CHECK(vec4->y == 1.0f);
    CHECK(vec4->z == 5.0f);
    CHECK(vec4->w == 2.0f);

    vec4 += vec4;
  }
}

TEST_CASE("Vectors to string output") {
  math::vec4 vec4 {1.0f, 0.0f, 5.0f, -7.0f};
  std::stringstream ss;

  ss << vec4;

  CHECK(ss.str() == "{ 1 0 5 -7 }");
}

TEST_CASE("Vectors operators") {
  math::vec2 vec2_1 {1.0, 2.0};
  math::vec2 vec2_2 {2.0, 3.0};
  math::vec3 vec3_1 {1.5, 2.0, 3.0};
  math::vec3 vec3_2 {5.0, 4.0, 3.0};
  math::vec4 vec4_1 {1.0, 2.0, 3.0, 4.0};
  math::vec4 vec4_2 {4.0, 3.0, 2.0, 1.0};

  DOCTEST_SUBCASE("Add") {
    DOCTEST_CHECK(vec2_1 + vec2_2 == math::vec2 {3.0, 5.0});
    DOCTEST_CHECK(vec3_1 + vec3_2 == math::vec3 {6.5, 6.0, 6.0});
    DOCTEST_CHECK(vec4_1 + vec4_2 == math::vec4 {5.0, 5.0, 5.0, 5.0});
  }

  DOCTEST_SUBCASE("Add Assignment") {
    math::vec2 v2 = vec2_1;
    v2 += vec2_2;
    DOCTEST_CHECK(v2 == math::vec2 {3.0, 5.0});

    math::vec3 v3 = vec3_1;
    v3 += vec3_2;
    DOCTEST_CHECK(v3 == math::vec3 {6.5, 6.0, 6.0});

    math::vec4 v4 = vec4_1;
    v4 += vec4_2;
    DOCTEST_CHECK(v4 == math::vec4 {5.0, 5.0, 5.0, 5.0});
  }

  DOCTEST_SUBCASE("Subtract") {
    DOCTEST_CHECK(vec2_1 - vec2_2 == math::vec2 {-1.0, -1.0});
    DOCTEST_CHECK(vec3_1 - vec3_2 == math::vec3 {-3.5, -2.0, 0.0});
    DOCTEST_CHECK(vec4_1 - vec4_2 == math::vec4 {-3.0, -1.0, 1.0, 3.0});
  }

  DOCTEST_SUBCASE("Subtract Assignment") {
    math::vec2 v2 = vec2_1;
    v2 -= vec2_2;
    DOCTEST_CHECK(v2 == math::vec2 {-1.0, -1.0});

    math::vec3 v3 = vec3_1;
    v3 -= vec3_2;
    DOCTEST_CHECK(v3 == math::vec3 {-3.5, -2.0, 0.0});

    math::vec4 v4 = vec4_1;
    v4 -= vec4_2;
    DOCTEST_CHECK(v4 == math::vec4 {-3.0, -1.0, 1.0, 3.0});
  }

  DOCTEST_SUBCASE("Multiply (Scalar)") {
    DOCTEST_CHECK(vec2_1 * 2.0f == math::vec2 {2.0, 4.0});
    DOCTEST_CHECK(vec3_1 * 2.0f == math::vec3 {3.0, 4.0, 6.0});
    DOCTEST_CHECK(vec4_1 * 2.0f == math::vec4 {2.0, 4.0, 6.0, 8.0});
  }

  DOCTEST_SUBCASE("Multiply Assignment (Scalar)") {
    math::vec2 v2 = vec2_1;
    v2 *= 2.0f;
    DOCTEST_CHECK(v2 == math::vec2 {2.0, 4.0});

    math::vec3 v3 = vec3_1;
    v3 *= 2.0f;
    DOCTEST_CHECK(v3 == math::vec3 {3.0, 4.0, 6.0});

    math::vec4 v4 = vec4_1;
    v4 *= 2.0f;
    DOCTEST_CHECK(v4 == math::vec4 {2.0, 4.0, 6.0, 8.0});
  }
}
