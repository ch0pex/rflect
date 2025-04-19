/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file proxy.hpp
 * @version 1.0
 * @date 28/03/2025
 * @brief Short description
 *
 * Longer description
 */

#pragma once

#include <rflect/converters/soa_to_zip.hpp>

#define PARENS ()

#define EXPAND(...) EXPAND4(EXPAND4(EXPAND4(EXPAND4(__VA_ARGS__))))
#define EXPAND4(...) EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__))))
#define EXPAND3(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND2(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND1(...) __VA_ARGS__

#define FOR_EACH(macro, ...) __VA_OPT__(EXPAND(FOR_EACH_HELPER(macro, __VA_ARGS__)))
#define FOR_EACH_HELPER(macro, a1, ...) macro(a1) __VA_OPT__(FOR_EACH_AGAIN PARENS(macro, __VA_ARGS__))
#define FOR_EACH_AGAIN() FOR_EACH_HELPER

#define EXPAND_PROXY_METHOD(name, ...)                                                                                 \
  auto name() -> decltype(auto) {                                                                                      \
    using namespace rflect;                                                                                            \
    return (this->template member<#name##_ss>());                                                                      \
  }                                                                                                                    \
  auto name() const -> decltype(auto) {                                                                                \
    using namespace rflect;                                                                                            \
    return (this->template member<#name##_ss>());                                                                      \
  }

#define DEFINE_PROXY_METHODS(name, ...)                                                                                \
  using rflect::proxy_base<name, Container>::operator=;                                                                \
  using rflect::proxy_base<name, Container>::proxy_base;                                                               \
  FOR_EACH(EXPAND_PROXY_METHOD, __VA_ARGS__)

#define DEFINE_PROXY(...)                                                                                              \
  template<typename Container>                                                                                         \
  struct proxy_type : rflect::proxy_base<proxy_type, Container> {                                                      \
    DEFINE_PROXY_METHODS(proxy_type, __VA_ARGS__)                                                                      \
  }

namespace rflect {

template<template<typename> class Proxy, class Container>
class proxy_base {
public:
  // *** Type traits ***
  using container            = Container;
  using proxy_type           = Proxy<container>;
  using value_type           = typename Container::value_type;
  using underlying_container = std::conditional_t<
      std::is_const_v<container>, typename Container::underlying_container const,
      typename Container::underlying_container>;

  // *** Constructors ***
  constexpr proxy_base(underlying_container& cont, std::size_t const index) : index_(index), container_(cont) { }

  constexpr explicit proxy_base(proxy_base const& other) = default;

  constexpr explicit proxy_base(proxy_base&& other) = default;

  constexpr ~proxy_base() = default;

  // *** Operators ***
  constexpr proxy_type& operator=(proxy_base&& other)  noexcept {
    container_ = other.container_;
    index_     = other.index_;
    return static_cast<proxy_type&>(*this);
  }

  constexpr proxy_type& operator=(value_type const& value)
    requires(aos_layout<container>)
  {
    container_.at(index_) = value;
    return static_cast<proxy_type&>(*this);
  }

  constexpr proxy_type& operator=(value_type const& value)
    requires(soa_layout<container>)
  {
    template for (constexpr auto member: nonstatic_data_members_of(^^value_type) | to_static_array) {
      constexpr auto identifier = std::meta::define_static_string(identifier_of(member));
      container_.template items<identifier>().at(index_) = value.[:member:];
    }
    return static_cast<proxy_type&>(*this);
  }

  constexpr proxy_type& operator=(proxy_base const& value)
    requires(aos_layout<container>)
  {
    if (this != &value) {
      container_.at(index_) = *static_cast<proxy_type const&>(value);
    }
    return static_cast<proxy_type&>(*this);
  }

  constexpr proxy_type& operator=(proxy_base const& value)
    requires(soa_layout<container>)
  {
    if (this == &value)
      return static_cast<proxy_type&>(*this);

    auto tuple          = *static_cast<proxy_type const&>(value);
    constexpr auto size = (nonstatic_data_members_of(^^underlying_container) | to_static_array).size();
    template for (constexpr auto index: static_iota<size>()) {
      container_.[:nonstatic_data_member<underlying_container>([:index:]):].at(index_) = std::get<[:index:]>(tuple);
    }
    return static_cast<proxy_type&>(*this);
  }


  template<typename Self>
  constexpr auto operator*(this Self&& self)
    requires(aos_layout<container>)
  {
    return self.container_.at(self.index_);
  }

  template<typename Self>
  constexpr auto operator*(this Self&& self)
    requires(soa_layout<container>)
  {
    return self.container_.as_zip()[self.index_];
  }

protected:
  template<char const* name, typename Self>
    requires(aos_layout<container>)
  constexpr auto member(this Self&& self) -> decltype(auto) {
    return (self.container_.at(self.index_).[:nonstatic_data_member<value_type>(name):]);
  }

  template<char const* name, typename Self>
    requires(soa_layout<container>)
  constexpr auto member(this Self&& self) -> decltype(auto) {
    return (self.container_.template items<name>().at(self.index_));
  }

private:
  std::size_t index_;
  underlying_container& container_;
};

} // namespace rflect
