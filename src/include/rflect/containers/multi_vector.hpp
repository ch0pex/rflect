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
  using iterator             = decltype(std::begin(std::declval<as_zip<underlying_container>>()));
  using const_iterator       = decltype(std::cbegin(std::declval<as_zip<underlying_container>>()));

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
    template for (constexpr auto member:
                  nonstatic_data_members_of(^^underlying_container, std::meta::access_context::unchecked()) |
                      to_static_array) {
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
  constexpr auto begin(this Self&& self) noexcept {
    return std::begin(soa_to_zip(std::forward<Self>(self).data_));
  }

  template<typename Self>
  constexpr auto end(this Self&& self) noexcept {
    return std::end(soa_to_zip(std::forward<Self>(self).data_));
  }

  constexpr auto cbegin() noexcept { return std::cbegin(data_ | soa_to_zip); }

  constexpr auto cend() noexcept { return std::cend(soa_to_zip(data_)); }

  // ********* Modifiers *********

  constexpr void push_back(value_type const& item) {
    template for (constexpr auto member:
                  nonstatic_data_members_of(^^underlying_container, std::meta::access_context::unchecked()) |
                      to_static_array) {
      data_.[:member:].push_back(item.[:nonstatic_data_member<value_type>(identifier_of(member)):]);
    }
  }

  constexpr void push_back(auto const value) {
    template for (constexpr auto index: static_iota<members_count>()) {
      data_.[:nonstatic_data_member<underlying_container>([:index:]):].push_back(std::get<[:index:]>(value));
    }
  }

  constexpr void pop_back() {
    template for (constexpr auto index: static_iota<members_count>()) {
      data_.[:nonstatic_data_member<underlying_container>([:index:]):].pop_back();
    }
  }

  constexpr auto erase(iterator const it) {
    auto const diff = it - cbegin();
    template for (constexpr auto index: static_iota<members_count>()) {
      constexpr auto member = nonstatic_data_member<underlying_container>([:index:]);
      data_.[:member:].erase(data_.[:member:].begin() + diff);
    }
    return begin() + diff;
  }

  constexpr auto erase(iterator const begin, iterator const end) {
    auto const diff_begin = begin - cbegin();
    auto const diff_end   = end - cbegin();
    template for (constexpr auto index: static_iota<members_count>()) {
      constexpr auto member = nonstatic_data_member<underlying_container>([:index:]);
      data_.[:member:].erase(data_.[:member:].begin() + diff_begin, data_.[:member:].begin() + diff_end);
    }
    return begin() + diff_end;
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
  static constexpr auto members_count =
      (nonstatic_data_members_of(^^underlying_container, std::meta::access_context::unchecked())).size();
  underlying_container data_ {};
};

} // namespace rflect
