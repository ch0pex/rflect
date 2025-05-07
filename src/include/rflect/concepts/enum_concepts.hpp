/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file enum_concepts.hpp
 * @version 1.0
 * @date 5/1/25
 * @brief Short description
 *
 * Longer description
 */
#pragma once

#include <experimental/meta>
#include <concepts>

namespace rflect {

template<typename E>
concept is_enum = std::is_enum_v<E>;

}