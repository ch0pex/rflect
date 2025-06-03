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

#include <concepts>
#include <experimental/meta>

namespace rflect {

/**
 * Concept to check if some type is an Enum with complete definition
 * @note is_enumerable_type checks if a type has complete definition despite it's confusing name imo :)
 */
template<typename E>
concept complete_enum = std::is_enum_v<E> and is_enumerable_type(^^E);

} // namespace rflect
