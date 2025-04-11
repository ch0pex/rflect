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

#include "utility.hpp"

namespace {

constexpr std::string_view iterator_error =  "Forward iterator concept not satisfied";
constexpr std::string_view range_error =  "Range_error not satisfied";

static_assert(std::forward_iterator<rflect::proxy_iterator<mock_proxy_vec>>, iterator_error);
static_assert(std::forward_iterator<rflect::proxy_iterator<bigger_mock_proxy_vec>>, iterator_error);
static_assert(std::ranges::range<rflect::dual_vector<Mock>>, range_error);
static_assert(std::ranges::range<rflect::dual_vector<BiggerMock>>, range_error);

// template<typename t>
// consteval void check_iterator(auto check) {
//   static_assert(std::forward_iterator<rflect::proxy_iterator<T>>, "rflect::proxy_iterator<T> must be a forward iterator");
// }
//
// template<typename... T>
// consteval void test_mock_types() {
//   (check_iterator<T>(), ...);
// }
//
// consteval {
//   test_mock_types<mock_proxy_vec, bigger_mock_proxy_vec>();
// }


}