/************************************************************************
 * Copyright (c) 2025 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/
/**
 * @file main.cpp
 * @version 1.0
 * @date 23/03/2025
 * @brief Short description
 *
 * Longer description
 */


#include "simulator.hpp"

#include <chrono>
#include <filesystem>
#include <iostream>
#include <source_location>
#include <span>
#include <utils/primitive_types.hpp>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <args/proargs.hpp>
#include <doctest/doctest.h>
#include <file/fld.hpp>

inline std::filesystem::path const current_path =
    std::filesystem::path(std::source_location::current().file_name()).parent_path();


namespace {

sim::error_code run_sim(std::span<char const*> const args) {
  auto const init = std::chrono::high_resolution_clock::now();

  sim::parse_arguments(args)
      .and_then(sim::run_simulation)
      .and_then(sim::write_output)
      .or_else([](std::runtime_error const& error) -> sim::err::expected<sim::Simulation> {
        std::println("Error: {}", error.what());
        return sim::err::unexpected(std::format("Simulation Failed", error.what()));
      });

  auto const end                                       = std::chrono::high_resolution_clock::now();
  std::chrono::duration<sim::math::scalar> const total = end - init;
  std::cout << "Execution time: " << total << "\n";
  std::cout << "---------------------------------------\n";

  return sim::success;
}

bool compare_files(std::string const& file1, std::string const& file2) {
  std::ifstream f1(file1, std::ios::binary);
  std::ifstream f2(file2, std::ios::binary);

  if (!f1 || !f2) {
    DOCTEST_MESSAGE("Couldn't open test files for comparing them...");
    return false;
  }

  DOCTEST_INFO(std::format("Comparing files: {} - {}", file1, file2));
  return std::equal(std::istreambuf_iterator(f1), std::istreambuf_iterator<char>(), std::istreambuf_iterator(f2));
}

void test_simulation(std::string_view file_name, sim::i8 iterations) {
  std::filesystem::path tmp_dir = "/tmp/testing_sim";
  create_directory(tmp_dir);
  std::string const expected_file = current_path / std::format("expected/{}-{}.fld", file_name, iterations);
  std::string const input_file    = current_path / std::format("input/{}.fld", file_name);
  std::string const out_file      = tmp_dir / std::format("{}-{}.fld", file_name, iterations);
  std::string const it            = std::to_string(iterations);

  std::array args {"binary_name", it.c_str(), input_file.c_str(), out_file.c_str()};
  DOCTEST_CHECK(run_sim(args) == sim::error_code::success);
  DOCTEST_CHECK(compare_files(out_file, expected_file));
  REQUIRE(std::filesystem::remove_all(tmp_dir));
}

} // namespace

TEST_CASE("Large file") {
  for (sim::i8 i = 1; i <= 5; ++i)
    test_simulation("large", i);
}

TEST_CASE("Small file") {
  for (sim::i8 i = 1; i <= 5; ++i)
    test_simulation("small", i);
}
