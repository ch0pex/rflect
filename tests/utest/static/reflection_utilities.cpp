/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details) 
 ************************************************************************/
/**
 * @file reflection_utilities.cpp
 * @version 1.0
 * @date 28/03/2025
 * @brief Short description
 *
 * Longer description
 */

#include "iterator.hpp"

#include <concepts>

#include "dual_vector.hpp"
#include "proxy_class.hpp"

namespace {

struct Mock {
  DEFINE_PROXY(id, density, velocity);

  std::int32_t id;
  std::double_t density;
  std::array<std::double_t, 3> velocity;
};

using proxy_t = Mock::proxy_type<acb::dual_vector<Mock>>;

static_assert(std::forward_iterator<acb::ProxyIterator<proxy_t>>);
static_assert(std::ranges::range<acb::dual_vector<Mock>>);

}