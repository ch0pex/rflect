#pragma once

#include <expected>
#include <stdexcept>
#include <string>

namespace sim {
/// Enum que contiene los distintos errores que puede arrojar el simulador
enum error_code {
  wrong_particle_number = -5,
  output_error          = -4,
  init_file_error,
  wrong_time_step,
  wrong_args,
  success
};


} // namespace sim

namespace sim::err {

template<typename T>
using expected = std::expected<T, std::runtime_error>;

constexpr auto unexpected = [](std::string_view const error_msg) {
  return std::unexpected {std::runtime_error {std::string {error_msg}}};
};

constexpr auto to_runtime_error = []<typename T>(std::unexpected<T> const& error) { return error.error(); };

} // namespace sim::err
