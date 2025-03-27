#pragma once

#include "utils/error.hpp"
#include "fld.hpp"

#include <charconv>
#include <span>

namespace sim {

inline std::optional<i32> parse_int(std::string_view const str) {
  int value;
  auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value);
  return (ec == std::errc()) ? std::optional {value} : std::nullopt;
}

inline err::expected<Simulation> parse_arguments(std::span<char const*> const args) {
  if (args.size() != 4) {
    return err::unexpected("Invalid number of arguments");
  }

  if (auto const value = parse_int(args.at(1))) {
    return read_input_file(Arguments {.iterations = *value, .input_file = args.at(2), .output_file = args.at(3)});
  }

  return err::unexpected("Could not parse arguments");
}

} // namespace sim
