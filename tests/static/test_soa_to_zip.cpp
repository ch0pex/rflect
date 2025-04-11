/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file test_soa_to_zip.hpp
 * @version 1.0
 * @date 4/9/25
 * @brief Short description
 *
 * Longer description
 */


#include "utility.hpp"

namespace {

// clang-format off
static_assert(std::same_as<decltype(std::views::zip(mock_vector.id, mock_vector.density, mock_vector.velocity)), rflect::as_zip<decltype(mock_vector)>>);
static_assert(std::same_as<decltype(std::views::zip(mock_array.id, mock_array.density, mock_array.velocity)), rflect::as_zip<decltype(mock_array)>>);

static_assert(std::same_as<decltype(std::views::zip(big_mock_array.id, big_mock_array.density, big_mock_array.velocity)), rflect::as_zip<decltype(mock_array)>>);
static_assert(std::same_as<decltype(std::views::zip(big_mock_vector.id, big_mock_vector.density, big_mock_vector.velocity)), rflect::as_zip<decltype(mock_vector)>>);

static_assert(not std::same_as<decltype(std::views::zip(big_mock_array.id, big_mock_array.density, big_mock_array.velocity)), rflect::as_zip<decltype(big_mock_array)>>);
static_assert(not std::same_as<decltype(std::views::zip(big_mock_vector.id, big_mock_vector.density, big_mock_vector.velocity)), rflect::as_zip<decltype(big_mock_vector)>>);

static_assert(std::same_as<decltype(std::views::zip(big_mock_array.id, big_mock_array.density, big_mock_array.velocity, big_mock_array.positions, big_mock_array.friends)), rflect::as_zip<decltype(big_mock_array)>>);
static_assert(std::same_as<decltype(std::views::zip(big_mock_vector.id, big_mock_vector.density, big_mock_vector.velocity, big_mock_vector.positions, big_mock_vector.friends)), rflect::as_zip<decltype(big_mock_vector)>>);
//clang-format on

}