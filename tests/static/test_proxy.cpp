/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file proxy_concepts.hpp
 * @version 1.0
 * @date 4/7/25
 * @brief Short description
 *
 * Longer description
 */

#include "utility.hpp"

#include <concepts>

namespace {

// is_proxyable
// has_proxy_type
// is_proxy_type

static_assert(std::same_as<std::int32_t&, decltype(std::declval<mock_proxy_vec>().id())>);
static_assert(std::same_as<std::double_t&, decltype(std::declval<mock_proxy_vec>().density())>);
static_assert(std::same_as<std::array<std::double_t, 3>&, decltype(std::declval<mock_proxy_vec>().velocity())>);

static_assert(std::same_as<std::int32_t&, decltype(std::declval<bigger_mock_proxy_vec>().id())>);
static_assert(std::same_as<std::double_t&, decltype(std::declval<bigger_mock_proxy_vec>().density())>);
static_assert(std::same_as<std::vector<std::double_t>&, decltype(std::declval<bigger_mock_proxy_vec>().velocity())>);
static_assert(std::same_as<std::vector<std::double_t>&, decltype(std::declval<bigger_mock_proxy_vec>().positions())>);
static_assert(std::same_as<std::array<Mock, 1024>&, decltype(std::declval<bigger_mock_proxy_vec>().friends())>);

}
