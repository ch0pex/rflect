/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file math_detail.hpp
 * @version 1.0
 * @date 12/03/2025
 * @brief Math details
 *
 * Math implementation details
 */

#pragma once

#include "reflection_utils.hpp"

#include <array>
#include <iostream>

namespace sim::math::detail {

constexpr std::array vector_components  = {"x"_ss, "y"_ss, "z"_ss, "w"_ss};
constexpr std::array color_components   = {"r"_ss, "g"_ss, "b"_ss, "a"_ss};
constexpr std::array texture_components = {"u"_ss, "v"_ss, "w"_ss};


/**
 * @class Vec
 * @tparam Dimension Vector dimension
 * @tparam T Vector elements type
 * @tparam Names Vector elements aliases
 *
 * This class is an algebraic vector that supports N dimensions using
 * static reflection to generate it's data members and operators
 */
template<u8 Dimension, std::floating_point T, std::ranges::range auto Names>
class Vec {
public:
  // *** Constructors ***
  Vec() = default;

  Vec(std::floating_point auto value) {
    template_for(member, nonstatic_data_members_of(^^data)) { data_.[:member:] = value; };
  }

  template<std::floating_point... Args>
    requires(sizeof...(Args) == Dimension)
  explicit constexpr Vec(Args... args) : data_(args...) { }

  // *** Operators ***
  template<typename Self>
  constexpr auto operator->(this Self&& self) {
    return &std::forward<Self>(self).data_;
  }

  constexpr auto operator+=(Vec const v2) {
    template_for(member, nonstatic_data_members_of(^^data)) {
      data_.[:member:] += v2.data_.[:member_named<data>(identifier_of(member)):];
    };
    return *this;
  }

  constexpr auto operator-=(Vec const v2) {
    template_for(member, nonstatic_data_members_of(^^data)) {
      data_.[:member:] -= v2.data_.[:member_named<data>(identifier_of(member)):];
    };
    return *this;
  }

  constexpr auto operator*=(Vec const v2) {
    template_for(member, nonstatic_data_members_of(^^data)) {
      data_.[:member:] *= v2.data_.[:member_named<data>(identifier_of(member)):];
    };
    return *this;
  }

  constexpr auto operator/=(Vec const v2) {
    template_for(member, nonstatic_data_members_of(^^data)) {
      data_.[:member:] /= v2.data_.[:member_named<data>(identifier_of(member)):];
    };
    return *this;
  }

  // *** Friend operators ***
  friend std::ostream& operator<<(std::ostream& os, Vec const obj) {
    os << "{ ";
    template_for(member, nonstatic_data_members_of(^^data)) { os << obj.data_.[:member:] << " "; };
    os << "}\n";
    return os;
  }

  friend constexpr bool operator==(Vec const vec, Vec const v2) {
    bool equals = true;
    template_for(member, nonstatic_data_members_of(^^data)) {
      if (vec.data_.[:member:] != v2.data_.[:member_named<data>(identifier_of(member)):]) {
        equals = false;
      }
    };
    return equals;
  }

  // clang-format off
  friend constexpr auto operator+(Vec lh, Vec const rh) { lh += rh; return lh; }

  friend constexpr auto operator-(Vec lh, Vec const rh) { lh -= rh; return lh; }

  friend constexpr auto operator*(Vec lh, Vec const rh) { lh *= rh; return lh; }

  friend constexpr auto operator/(Vec lh, Vec const rh) { lh /= rh; return lh; }
  // clang-format on

private:
  // *** Data type forward declaration ***
  struct data;

  // *** Data type generation with consteval block feature
  consteval {
    std::vector<std::meta::info> members;
    members.reserve(Names.size());

    for (auto name: Names) { // clang-format off
      members.push_back(std::meta::data_member_spec(^^T, { .name = name }));
    } // clang-format on
    define_aggregate(^^data, members);
  }

  data data_;
};

template<typename T>
struct is_vector : std::false_type { };

template<std::floating_point T, u8 Dimension, std::ranges::range auto Names>
struct is_vector<Vec<Dimension, T, Names>> : std::true_type { };

} // namespace sim::math::detail
