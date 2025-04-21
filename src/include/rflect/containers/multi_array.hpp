/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file multi_array.hpp
 * @version 1.0
 * @date 4/3/25
 * @brief MultiArray class
 */
#pragma once

#include <rflect/converters/soa_to_zip.hpp>
#include <rflect/converters/struct_to_soa.hpp>
#include <rflect/converters/to_static.hpp>
#include <rflect/introspection/struct.hpp>

namespace rflect {

template<typename T, std::size_t N>
  requires(std::is_aggregate_v<T>)
class multi_array {
public:
  /**********************************
   *          Member types          *
   **********************************/
  using value_type           = T;
  using underlying_container = struct_of_arrays<T, N>;

  /**********************************
   *        Member functions        *
   **********************************/

  // ********* Constructors *********

  constexpr multi_array() = default;

  constexpr multi_array(std::initializer_list<value_type> init) {
    for (std::size_t i = 0; auto const& item: init) {
      template for (constexpr auto member : nonstatic_data_members_of(^^underlying_container) | to_static_array) {
        data_.[:member:][i] = item.[:nonstatic_data_member<value_type>(identifier_of(member)):];
      }
      ++i;
    }
  }

  // ********* Element access *********

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

  template<std::size_t Idx, typename Self>
  constexpr decltype(auto) items(this Self& self) {
    return (self.data_.[:nonstatic_data_member<underlying_container>(Idx):]);
  }

  template<char const* name, typename Self>
  constexpr decltype(auto) items(this Self& self) {
    return (self.data_.[:nonstatic_data_member<underlying_container>(name):]);
  }

  constexpr auto as_zip() { return soa_to_zip(data_); }

  // ********* Iterators *********

  template<typename Self>
  constexpr auto begin(this Self self) {
    return std::begin(soa_to_zip(self.data_));
  }

  template<typename Self>
  constexpr auto end(this Self self) {
    return std::end(soa_to_zip(self.data_));
  }

  constexpr auto cbegin() const {
    return std::cbegin(soa_to_zip(data_));
  }

  constexpr auto cend() {
    return std::cend(soa_to_zip(data_));
  }

  // ********* Capacity *********

  [[nodiscard]] constexpr std::size_t size() const {
    return data_.[:nonstatic_data_member<underlying_container>(0):].size();
  }

  [[nodiscard]] constexpr std::size_t max_size() const {
    return data_.[:nonstatic_data_member<underlying_container>(0):].max_size();
  }

  [[nodiscard]] constexpr std::size_t empty() const {
    return data_.[:nonstatic_data_member<underlying_container>(0):].empty();
  }

private:
  underlying_container data_ {};
};

}