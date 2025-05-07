/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file multi_vector.hpp
 * @version 1.0
 * @date 4/3/25
 * @brief Multi vector class
 */

#pragma once

#include <rflect/converters/soa_to_zip.hpp>
#include <rflect/converters/struct_to_soa.hpp>
#include <rflect/converters/to_static.hpp>
#include <rflect/introspection/struct.hpp>

namespace rflect {

/**
 * @brief Container abstracting struct of vectors conversion
 *
 * This class converts an aggregate type into a structure of vectors (SoA - Structure of Arrays), encapsulating the
 * underlying complexity. It offers a simple and intuitive public interface that closely resembles standard containers
 * (e.g., std::vector), making it easy to use and integrate.
 *
 * @tparam T Aggregate type to be converted
 * @tparam Alloc Allocator type for vectors
 */
template<typename T, template<typename> class Alloc = std::allocator>
  requires(std::is_aggregate_v<T>) // TODO concept this type trait
class multi_vector {
public:
  /**********************************
   *          Member types          *
   **********************************/
  using value_type           = T;
  using underlying_container = struct_of_vectors<T, Alloc>;

  /**********************************
   *        Member functions        *
   **********************************/

  // ********* Constructors *********

  constexpr multi_vector() = default;

  constexpr multi_vector(std::initializer_list<value_type> init) {
    for (auto const& item: init) {
      push_back(item);
    }
  }

  constexpr explicit multi_vector(std::integral auto size) {
    template for (constexpr auto member: nonstatic_data_members_of(^^underlying_container) | to_static_array) {
      data_.[:member:] = decltype(data_.[:member:])(size);
    }
  }

  // ********** Element access **********

  template<typename Self>
  constexpr auto at(this Self self, std::size_t const index) {
    static auto zip = soa_to_zip(self.data_);
    return zip[index];
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
  constexpr auto back(this Self self) {
    return soa_to_zip(self.data_)[self.size() - 1];
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

  // ********* Iterators *********

  template<typename Self>
  constexpr auto begin(this Self self) noexcept {
    return std::begin(soa_to_zip(self.data_));
  }

  template<typename Self>
  constexpr auto end(this Self self) noexcept {
    return std::end(soa_to_zip(self.data_));
  }

  constexpr auto cbegin() noexcept { return std::cbegin(soa_to_zip(data_)); }

  constexpr auto cend() noexcept { return std::cend(soa_to_zip(data_)); }

  // ********* Modifiers *********

  constexpr void push_back(value_type const& item) {
    template for (constexpr auto member: nonstatic_data_members_of(^^underlying_container) | to_static_array) {
      data_.[:member:].push_back(item.[:nonstatic_data_member<value_type>(identifier_of(member)):]);
    }
  }

  constexpr void push_back(auto const value) {
    static constexpr auto size = (nonstatic_data_members_of(^^underlying_container)).size();
    template for (constexpr auto index: static_iota<size>()) {
      data_.[:nonstatic_data_member<underlying_container>([:index:]):].push_back(std::get<[:index:]>(value));
    }
  }

  // ********* Capacity *********

  [[nodiscard]] constexpr std::size_t empty() const noexcept {
    return data_.[:nonstatic_data_member<underlying_container>(0):].empty();
  }

  [[nodiscard]] constexpr std::size_t size() const noexcept {
    return data_.[:nonstatic_data_member<underlying_container>(0):].size();
  }

  [[nodiscard]] constexpr std::size_t max_size() const noexcept {
    return data_.[:nonstatic_data_member<underlying_container>(0):].max_size();
  }

  [[nodiscard]] constexpr std::size_t capacity() const noexcept {
    return data_.[:nonstatic_data_member<underlying_container>(0):].capacity();
  }

private:
  underlying_container data_ {};
};

} // namespace rflect
