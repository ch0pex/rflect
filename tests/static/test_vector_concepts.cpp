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

#include <rflect/containers/dual_vector.hpp>
#include <rflect/containers/proxy.hpp>

#include <concepts>

namespace {

struct Mock {
  DEFINE_PROXY(id, density, velocity);

  std::int32_t id;
  std::double_t density;
  std::array<std::double_t, 3> velocity;
};

using proxy_t = Mock::proxy_type<rflect::dual_vector<Mock>>;

static_assert(std::forward_iterator<rflect::proxy_iterator<proxy_t>>, "proxy_iterator doesn't satisfy forward_iterator ");
static_assert(std::ranges::range<rflect::dual_vector<Mock>>, "ProxyIterator doesn't satisfy forward_iterator ");

}