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

template<is_enum E>
constexpr auto get_pairs() {
  return std::meta::enumerators_of(^^E) | std::views::transform([](std::meta::info e) {
           return std::pair<E, std::string>(std::meta::extract<E>(e), std::meta::identifier_of(e));
         });
};

template<is_enum E>
constexpr auto find_pair(E value) -> std::optional<std::pair<E, std::string>> {
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

} // namespace detail

/**
 * @brief Converts an enum value to its corresponding name as a string.
 *
 * @tparam E The enum type to introspect. Must satisfy `std::is_enum_v<E>`.
 * @param value The enum value to be converted to a string.
 * @return The name of the enumerator as a `std::string`, or "<unnamed>" if no match is found.
 */
template<is_enum E>
consteval std::string enum_name(E const value) {
  return detail::find_pair<E>(value) //
      .transform([](auto const& pair) { return pair.second; }) //
      .value_or("<unnamed>");
}

/**
 * @brief Returns enum value at specified index
 *
 * @tparam E The enum type to introspect. Must satisfy `is_enum concept`.
 * @param index The zero-based index of the enumerator.
 * @return The enum value at the specified index.
 * @note out of bounds element is unspecified behavour
 */
template<is_enum E>
consteval E enum_value(std::size_t const index) {
  return extract<E>(std::meta::enumerators_of(^^E)[index]);
}

/**
 * @brief If exists, returns the value of the corresponding name in the specified
 * enumerator, otherwise returns std::nullopt
 *
 * @tparam E Enumerator type. Must satisfy `is_enum concept`.
 * @tparam BinaryPredicate The type of the predicate used for name comparison. Defaults to `std::equal_to<>`.
 * @param name Name to be converted as enumerator.
 * @param predicate Binary predicate to comparison. Defaults to `std::equal_to<>{}`.
 * @return optional enumerate value wrapped in `std::optional<E>`.
 */
template<is_enum E, typename BinaryPredicate = std::equal_to<>>
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
 * @tparam E Enumerator type. Must satisfy `is_enum concept`.
 * @tparam Integral The type of the integral number.
 * @tparam BinaryPredicate The type of the predicate used for value comparison. Defaults to `std::equal_to<>`.
 * @param number The integral number to be cast to an enumerator.
 * @param predicate Binary predicate to comparison. Defaults to `std::equal_to<>{}`.
 * @return optional enumerate value wrapped in `std::optional<E>`.
 */
template<is_enum E, typename BinaryPredicate = std::equal_to<>>
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
 * @tparam E The enum type to introspect. Must satisfy `is_enum concept`.
 * @return The count of enumerators as a `std::size_t`.
 */
template<is_enum E>
consteval std::size_t enum_count() {
  return enumerators_of(^^E).size();
}

template<is_enum E>
consteval std::string_view enum_type_name() {
  return display_string_of(type_of(^^E));
}

template<is_enum E>
constexpr auto enum_values() {
  static constexpr auto values = enumerators_of(^^E) | std::views::transform(std::meta::extract<E>) | to_static_array;
  return values;
}

template<is_enum E>
constexpr auto enum_names() {
  static constexpr auto names = enumerators_of(^^E) | std::views::transform(std::meta::identifier_of) | to_static_array;
  return names;
}

} // namespace rflect
