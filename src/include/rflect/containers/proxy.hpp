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

namespace rflect {

/**
 * @brief Macros for generating proxy classes with methods that provide unified access to container elements.
 *
 * These macros are used to simplify the creation of proxy classes that interact with containers, enabling
 * layout-agnostic operations (e.g., assignment, dereferencing, member access) for both AoS (Array of Structures)
 * and SoA (Structure of Arrays) layouts. They allow the automatic generation of proxy methods, reducing boilerplate code
 * and providing a flexible interface for interacting with container elements.
 *
 * - `EXPAND`, `EXPAND1` through `EXPAND4`: Recursively expand macro arguments to ensure full macro expansion.
 * - `FOR_EACH`: Iterates over variadic arguments, applying a macro to each argument.
 * - `EXPAND_PROXY_METHOD`: Defines a proxy method for a given name.
 * - `DEFINE_PROXY_METHODS`: Defines multiple proxy methods for a proxy class.
 * - `DEFINE_PROXY`: Defines a `proxy_type` template that inherits from `proxy_base` and generates proxy methods.
 *
 * These macros help create proxy classes that can be easily extended and customized based on container layout,
 * simplifying interactions with container elements and improving code maintainability.
 */
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

/**
 * @brief Base class for proxy types that provide unified access to container elements.
 *
 * This CRTP (Curiously Recurring Template Pattern) base class enables proxy types to interact
 * with containers that follow either AoS (Array of Structures) or SoA (Structure of Arrays) layouts.
 * It handles assignment, dereferencing, and member access in a layout-agnostic way by forwarding
 * operations to the appropriate container interface.
 *
 * Users can inherit this class and build manually its proxy class or just use the provided macros to do such a
 * thing
 *
 * @tparam Proxy Proxy derived type (CRTP).
 * @tparam Container Container type from which elements are accessed (either AoS or SoA).
 *
 * @note This CRTP base class serves as a workaround due to current limitations in the static reflection proposal.
 * Ideally, the proxy would be implemented as a single template class with auto-generated member functions
 * using the token sequence code injection proposal. Until such language features are available, CRTP is used
 * to inject layout-specific behavior into the proxy class.
 */
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
  constexpr proxy_type& operator=(proxy_base&& other) noexcept {
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
      constexpr auto identifier                          = std::meta::define_static_string(identifier_of(member));
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

/**************************************************************************
 * Theoretical implementation of a proxy class with token                 *
 * sequence injection proposal (P3294R2).                                 *
 * This is the desirable approach for a fully autogenerated proxy class   *
 **************************************************************************/
/**
 * template<typename Container>
 * class proxy {
 * public:
 *   // *** Type traits ***
 *   using container            = Container;
 *   using value_type           = typename Container::value_type;
 *   using underlying_container = std::conditional_t<
 *     std::is_const_v<container>, typename Container::underlying_container const,
 *     typename Container::underlying_container>;
 *
 *   // *** Constructors ***
 *   constexpr proxy(underlying_container& cont, std::size_t const index) : index_(index), container_(cont) { }
 *
 *   constexpr explicit proxy(proxy const& other) = default;
 *
 *   constexpr explicit proxy(proxy&& other) = default;
 *
 *   constexpr ~proxy() = default;
 *
 *   // *** Getters ***
 *   consteval {
 *     template for (auto identifier : nonstatic_data_members_of(^^value_type) | std::views::transform(std::meta::identifier_of)) {
 *       proxy_member(identifier);
 *     }
 *   }
 *
 *   // *** Operators ***
 *   constexpr proxy& operator=(proxy&& other) noexcept {
 *     container_ = other.container_;
 *     index_     = other.index_;
 *     return *this;
 *   }
 *
 *   constexpr proxy& operator=(value_type const& value)
 *     requires(aos_layout<container>)
 *   {
 *     container_.at(index_) = value;
 *     return *this;
 *   }
 *
 *   constexpr proxy& operator=(value_type const& value)
 *     requires(soa_layout<container>)
 *   {
 *     template for (constexpr auto member: nonstatic_data_members_of(^^value_type) | to_static_array) {
 *       constexpr auto identifier                          = std::meta::define_static_string(identifier_of(member));
 *       container_.template items<identifier>().at(index_) = value.[:member:];
 *     }
 *     return *this;
 *   }
 *
 *   constexpr proxy& operator=(proxy const& value)
 *    requires(aos_layout<container>)
 *   {
 *     if (this != &value) {
 *       container_.at(index_) = *static_cast<proxy_type const&>(value);
 *     }
 *     return *this;
 *   }
 *
 *   constexpr proxy& operator=(proxy const& value)
 *    requires(soa_layout<container>)
 *   {
 *     if (this == &value)
 *       return *this;
 *
 *     auto tuple          = *value;
 *     constexpr auto size = (nonstatic_data_members_of(^^underlying_container) | to_static_array).size();
 *     template for (constexpr auto index: static_iota<size>()) {
 *       container_.[:nonstatic_data_member<underlying_container>([:index:]):].at(index_) = std::get<[:index:]>(tuple);
 *     }
 *     return *this;
 *   }
 *
 *
 *   template<typename Self>
 *   constexpr auto operator*(this Self&& self)
 *     requires(aos_layout<container>)
 *   {
 *     return self.container_.at(self.index_);
 *   }
 *
 *   template<typename Self>
 *   constexpr auto operator*(this Self&& self)
 *     requires(soa_layout<container>)
 *   {
 *     return self.container_.as_zip()[self.index_];
 *   }
 *
 * private:
 *   consteval auto proxy_member(std::string_view const name) -> void
 *     requires(aos_layout<container>)
 *   {
 *     queue_injection(^{
 *       template<typename Self>
 *       auto \id(name)(this Self&& self) -> decltype(auto) {
 *          return (self.container_.at(self.index_).[:nonstatic_data_member<value_type>(name):]);
 *       }
 *     });
 *   }
 *
 *   consteval auto proxy_member(std::string_view const name) -> void
 *     requires(soa_layout<container>)
 *   {
 *     queue_injection(^{
 *       template<typename Self>
 *       auto \id(name)(this Self&& self) -> decltype(auto) {
 *           return (self.container_.template items<name>().at(self.index_));
 *       }
 *     });
 *   }
 *
 *   std::size_t index_;
 *   std::optional<underlying_container&> container_;
 * };
 *
 */

} // namespace rflect
