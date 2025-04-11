/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file converter_closure.hpp
 * @version 1.0
 * @date 4/11/25
 * @brief Short description
 *
 * Longer description
 */
#pragma once

#include <experimental/meta>

namespace rflect {
template<typename Tp>
  requires std::is_class_v<Tp> and std::same_as<Tp, std::remove_cv_t<Tp>>
struct converter_closure { };

template<class Tp>
concept converter = std::is_base_of_v<converter_closure<Tp>, Tp>;

template<std::ranges::input_range R, converter Cv>
consteval auto operator|(R&& r, Cv const& self) {
  return self(std::forward<R>(r));
}

}
