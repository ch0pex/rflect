/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details) 
 ************************************************************************/
/**
 * @file layout_concepts.hpp
 * @version 1.0
 * @date 28/03/2025
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include <rflect/containers/memory_layout.hpp>
#include <concepts>

namespace rflect {

namespace detail {

template<typename T>
struct is_layout : std::false_type {};

template<>
struct is_layout<layout::soa> : std::true_type {};

template<>
struct is_layout<layout::aos> : std::true_type {};

}

template<typename T>
concept memory_layout = detail::is_layout<T>::value;

template<typename T>
concept soa_layout = std::same_as<T, layout::soa> or std::same_as<typename T::memory_layout, layout::soa>;

template<typename T>
concept aos_layout = std::same_as<T, layout::aos> or std::same_as<typename T::memory_layout, layout::aos>;

}
