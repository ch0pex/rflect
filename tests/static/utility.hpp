/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file utility.hpp
 * @version 1.0
 * @date 4/7/25
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include <rflect/containers.hpp>

struct Mock {
  DEFINE_PROXY(id, density, velocity);

  std::int32_t id;
  std::double_t density;
  std::array<std::double_t, 3> velocity;
};

using mock_proxy_vec = Mock::proxy_type<rflect::dual_vector<Mock>>;
// using mock_proxy_array = Mock::proxy_type<rflect::dual_array<Mock>>;

struct BiggerMock {
  DEFINE_PROXY(id, density, velocity, positions, friends);

  std::int32_t id;
  std::double_t density;
  std::vector<std::double_t> velocity;
  std::vector<std::double_t> positions;
  std::array<Mock, 1024> friends;
};

using bigger_mock_proxy_vec = BiggerMock::proxy_type<rflect::dual_vector<BiggerMock>>;
// using bigger_mock_proxy_array = BiggerMock::proxy_type<rflect::dual_array<Mock>>;
