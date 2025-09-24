/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file enum.hpp
 * @version 1.0
 * @date 4/11/25
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include <locale>
#include <rflect/concepts/enum_concepts.hpp>
#include <rflect/converters/to_static.hpp>

#include <experimental/meta>
#include <map>
#include <ranges>

namespace rflect {

namespace detail {

template<complete_enum E>
consteval auto get_pairs() {
  return enumerators_of(^^E) | std::views::transform([](std::meta::info e) {
           return std::pair<E, std::string_view>(extract<E>(e), identifier_of(e));
         });
};

template<complete_enum E>
consteval auto find_pair(E value) -> std::optional<std::pair<E, std::string_view>> {
  constexpr auto size = enumerators_of(^^E).size();
  if constexpr (size <= 7) {
    // if there aren't many enumerators, use a vector with find_if()
    auto enumerators = get_pairs<E>() | std::ranges::to<std::vector>();
    auto it          = std::ranges::find_if(enumerators, [value](auto const& pr) { return pr.first == value; });
    if (it == enumerators.end()) {
      return std::nullopt;
    }
    return *it;
  }
  else {
    // if there are lots of enumerators, use a map with find()
    auto enumerators = get_pairs<E>() | std::ranges::to<std::map>();
    auto it          = enumerators.find(value);
    if (it == enumerators.end()) {
      return std::nullopt;
    }
    return *it;
  }
}

template <auto V, typename F>
void invoke_enum(F&& f) {
  f(std::integral_constant<decltype(V), V>{});
}

} // namespace detail

/**
 * @brief Converts an enum value to its corresponding name as a string.
 *
 * @tparam E The enum type to introspect. Must satisfy `std::complete_enum_v<E>`.
 * @param value The enum value to be converted to a string.
 * @return The name of the enumerator as a `std::string`, or "<unnamed>" if no match is found.
 */
template<complete_enum E>
constexpr std::string_view enum_name(E const value) {
  static constexpr auto unnamed = "<unnamed>";
  if consteval {
    return detail::find_pair<E>(value) //
        .transform([](auto const& pair) { return pair.second; }) //
        .value_or(unnamed);
  }
  static constexpr auto enumerators = enumerators_of(^^E) | to_static_array;
  template for (constexpr auto e: enumerators) {
    if (value == [:e:])
      return identifier_of(e);
  }
  return unnamed;
}

/**
 * @brief Returns enum value at specified index
 *
 * @tparam E The enum type to introspect. Must satisfy `complete_enum concept`.
 * @param index The zero-based index of the enumerator.
 * @return The enum value at the specified index.
 * @note if the value is out of bounds, returns the maximum value of the underlying type.
 */
template<complete_enum E>
constexpr E enum_value(std::size_t const index) {
  if consteval {
    return extract<E>(enumerators_of(^^E)[index]);
  }

  std::size_t i = 0;
  static constexpr auto enumerators = enumerators_of(^^E) | to_static_array;
  template for (constexpr auto e: enumerators) {
    if (i++ == index)
      return [:e:];
  }

  return static_cast<E>(std::numeric_limits<std::underlying_type_t<E>>::max());
}

/**
 * @brief If exists, returns the value of the corresponding name in the specified
 * enumerator, otherwise returns std::nullopt
 *
 * @tparam E Enumerator type. Must satisfy `complete_enum concept`.
 * @tparam BinaryPredicate The type of the predicate used for name comparison. Defaults to `std::equal_to<>`.
 * @param name Name to be converted as enumerator.
 * @param predicate Binary predicate to comparison. Defaults to `std::equal_to<>{}`.
 * @return optional enumerate value wrapped in `std::optional<E>`.
 */
template<complete_enum E, typename BinaryPredicate = std::equal_to<>>
constexpr std::optional<E> enum_cast(std::string_view name, BinaryPredicate const predicate = {}) {
  if constexpr (is_enumerable_type(^^E)) {
    template for (constexpr auto e: std::meta::enumerators_of(^^E) | to_static_array) {
      if (predicate(name, std::meta::identifier_of(e)))
        return [:e:];
    }
  }
  return std::nullopt;
}

/**
 * @brief If exists, returns the enum value corresponding to an integral number
 * if a matching enumerator's underlying value equals the number, otherwise returns std::nullopt.
 *
 * @tparam E Enumerator type. Must satisfy `complete_enum concept`.
 * @tparam Integral The type of the integral number.
 * @tparam BinaryPredicate The type of the predicate used for value comparison. Defaults to `std::equal_to<>`.
 * @param number The integral number to be cast to an enumerator.
 * @param predicate Binary predicate to comparison. Defaults to `std::equal_to<>{}`.
 * @return optional enumerate value wrapped in `std::optional<E>`.
 */
template<complete_enum E, typename BinaryPredicate = std::equal_to<>>
constexpr std::optional<E> enum_cast(std::integral auto number, BinaryPredicate const predicate = {}) {
  if constexpr (is_enumerable_type(^^E)) {
    template for (constexpr auto e: std::meta::enumerators_of(^^E) | to_static_array) {
      if (predicate(static_cast<E>(number), std::meta::extract<E>(e)))
        return [:e:];
    }
  }
  return std::nullopt;
}

/**
 * @brief Returns the total number of enumerators in the specified enum type.
 *
 * @tparam E The enum type to introspect. Must satisfy `complete_enum concept`.
 * @return The count of enumerators as a `std::size_t`.
 */
template<complete_enum E>
consteval std::size_t enum_count() {
  return enumerators_of(^^E).size();
}

template<complete_enum E>
consteval std::string_view enum_type_name() {
  return display_string_of(type_of(^^E));
}

template<complete_enum E>
consteval auto enum_values() {
  static constexpr auto values = enumerators_of(^^E) | std::views::transform(std::meta::extract<E>);
  return values;
}

template<complete_enum E>
consteval auto enum_names() {
  static constexpr auto names = enumerators_of(^^E) | std::views::transform(std::meta::identifier_of);
  return names;
}

template<complete_enum E>
constexpr void enum_switch(auto&& func, E value) {
  static constexpr auto enums = enumerators_of(^^E) | std::views::transform(std::meta::extract<E>) | to_static_array;
  template for (constexpr auto e : enums) {
    if (e == value) {
      detail::invoke_enum<e>(func);
    }
  }

}

} // namespace rflect
