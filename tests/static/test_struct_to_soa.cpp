/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file struct_to_soa.hpp
 * @version 1.0
 * @date 4/9/25
 * @brief Short description
 *
 * Longer description
 */


#include "utility.hpp"

namespace {

// Struct of array conversion asserts (Mock)
static_assert(std::same_as<decltype(mock_array.id), std::array<decltype(Mock{}.id), 50>>);
static_assert(std::same_as<decltype(mock_array.density), std::array<decltype(Mock{}.density), 50>>);
static_assert(std::same_as<decltype(mock_array.velocity), std::array<decltype(Mock{}.velocity), 50>>);

// Struct of vector conversion asserts (Mock)
static_assert(std::same_as<decltype(mock_vector.id), std::vector<decltype(Mock{}.id)>>);
static_assert(std::same_as<decltype(mock_vector.density), std::vector<decltype(Mock{}.density)>>);
static_assert(std::same_as<decltype(mock_vector.velocity), std::vector<decltype(Mock{}.velocity)>>);

// Struct of array conversion asserts (BiggerMock)
static_assert(std::same_as<decltype(big_mock_array.id), std::array<decltype(BiggerMock{}.id), 50>>);
static_assert(std::same_as<decltype(big_mock_array.density), std::array<decltype(BiggerMock{}.density), 50>>);
static_assert(std::same_as<decltype(big_mock_array.velocity), std::array<decltype(BiggerMock{}.velocity), 50>>);
static_assert(std::same_as<decltype(big_mock_array.positions), std::array<decltype(BiggerMock{}.positions), 50>>);
static_assert(std::same_as<decltype(big_mock_array.friends), std::array<decltype(BiggerMock{}.friends), 50>>); // Usa ::value_type para Mock

// Struct of vector conversion asserts (BiggerMock)
static_assert(std::same_as<decltype(big_mock_vector.id), std::vector<decltype(BiggerMock{}.id)>>);
static_assert(std::same_as<decltype(big_mock_vector.density), std::vector<decltype(BiggerMock{}.density)>>);
static_assert(std::same_as<decltype(big_mock_vector.velocity), std::vector<decltype(BiggerMock{}.velocity)>>);
static_assert(std::same_as<decltype(big_mock_vector.positions), std::vector<decltype(BiggerMock{}.positions)>>);
static_assert(std::same_as<decltype(big_mock_vector.friends), std::vector<decltype(BiggerMock{}.friends)>>);

// TODO asserts for custom allocator types

} // namespace
