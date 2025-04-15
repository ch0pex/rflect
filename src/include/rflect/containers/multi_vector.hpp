/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file multi_vector.hpp
 * @version 1.0
 * @date 4/3/25
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include <rflect/converters/soa_to_zip.hpp>
#include <rflect/converters/to_static.hpp>
#include <rflect/introspection/struct.hpp>

namespace rflect {


template<typename T, template<typename> class Alloc = std::allocator>
class multi_vector {
public:
  // ********** Type traits **********
  using value_type           = T;
  using underlying_container = struct_of_vectors<T, Alloc>;

  // ********** Constructors **********
  constexpr multi_vector() = default;

  constexpr multi_vector(std::initializer_list<value_type> init) {
    for (auto const& item: init) {
      push_back(item);
    }
  }

  constexpr multi_vector(std::integral auto size) {
    template for (constexpr auto member: nonstatic_data_members_of(^^underlying_container) | to_static_array) {
      data_.[:member:] = decltype(data_.[:member:])(size);
    }
  }
  // ********** Member functions **********

  template<typename Self>
  constexpr auto at(this Self self, std::size_t const index) {
    return soa_to_zip(self.data_)[index];
  }

  template<typename Self>
  constexpr auto operator[](this Self self, std::size_t const index) {
    return self.at(index);
  }

  template<typename Self>
  constexpr auto front(this Self self) {
    return soa_to_zip(self.data_)[0];
  }

  template<typename Self>
  constexpr auto begin(this Self self) {
    return std::begin(soa_to_zip(self.data_));
  }

  template<typename Self>
  constexpr auto end(this Self self) {
    return std::end(soa_to_zip(self.data_));
  }

  [[nodiscard]] constexpr std::size_t size() const {
    return data_.[:nonstatic_data_member<underlying_container>(0):].size();
  }

  constexpr void push_back(value_type const& item) {
    template for (constexpr auto member: nonstatic_data_members_of(^^underlying_container) | to_static_array) {
      data_.[:member:].push_back(item.[:nonstatic_data_member<value_type>(identifier_of(member)):]);
    }
  }

  constexpr void push_back(auto const value) {
    constexpr auto size = (nonstatic_data_members_of(^^underlying_container) | to_static_array).size();
    template for (constexpr auto index: static_iota<size>()) {
      data_.[:nonstatic_data_member<underlying_container>([:index:]):].push_back(std::get<[:index:]>(value));
    }
  }

  template<std::size_t N, typename Self>
  constexpr decltype(auto) items(this Self& self) {
    return (self.data_.[:nonstatic_data_member<underlying_container>(N):]);
  }

  template<char const* name, typename Self>
  constexpr decltype(auto) items(this Self& self) {
    return (self.data_.[:nonstatic_data_member<underlying_container>(name):]);
  }

  constexpr auto as_zip() { return soa_to_zip(data_); }

  friend constexpr bool operator==(multi_vector const& vec1, multi_vector const& vec2) {
    bool equal = true;
    template for (constexpr auto member: nonstatic_data_members_of(^^underlying_container) | to_static_array) {
      equal &= (vec1.data_.[:member:] == vec2.data_.[:member:]);
    };
    return equal;
  }

private:
  underlying_container data_;
};

} // namespace rflect
