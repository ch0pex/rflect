#include "simulator.hpp"

#include <chrono>
#include <filesystem>
#include <iostream>
#include <source_location>
#include <span>
#include <utils/primitive_types.hpp>


#include <args/proargs.hpp>
#include <fld.hpp>

inline std::filesystem::path const current_path =
    std::filesystem::path(std::source_location::current().file_name()).parent_path();

inline auto run_sim(std::span<char const*> const args) {
  auto const init = std::chrono::high_resolution_clock::now();

  auto result = //
      sim::parse_arguments(args)
          .and_then(sim::run_simulation)
          .and_then(sim::write_output)
          .transform_error([](std::runtime_error const& error) -> std::string {
            return std::format("Simulation Failed!\n Error: {}", error.what());
          });

  if (not result) {
    std::cerr << "{}", result.error();
    return -1;
  }

  std::chrono::duration<sim::math::scalar> const total = std::chrono::high_resolution_clock::now() - init;
  std::cout << "Execution time: " << total << "\n";

  return 0;
}

inline bool compare_files(std::string const& file1, std::string const& file2) {
  std::ifstream f1(file1, std::ios::binary);
  std::ifstream f2(file2, std::ios::binary);

  if (!f1 || !f2) {
    std::cerr << "Couldn't open test files for comparing them\n";
    return false;
  }

  std::cout << std::format("Comparing files: {} - {}", file1, file2) << "\n";
  return std::equal(std::istreambuf_iterator(f1), std::istreambuf_iterator<char>(), std::istreambuf_iterator(f2));
}

inline void test_simulation(std::string_view file_name, sim::i32 iterations) {
  std::filesystem::path tmp_dir = "/tmp/testing_sim";
  create_directory(tmp_dir);
  std::string const expected_file = current_path / std::format("expected/{}-{}.fld", file_name, iterations);
  std::string const input_file    = current_path / std::format("input/{}.fld", file_name);
  std::string const out_file      = tmp_dir / std::format("{}-{}.fld", file_name, iterations);
  std::string const it            = std::to_string(iterations);

  std::array args {"binary_name", it.c_str(), input_file.c_str(), out_file.c_str()};

  std::cout << std::format("Number of iterations: {}\n", iterations);
  if (run_sim(args) != sim::error_code::success) {
    std::cout << "Sim incorrect :( ";
    return;
  }

  if (iterations <= 5) {
    std::string const result_string = compare_files(out_file, expected_file) ? "Sim correct\n" : "Sim incorrect :(\n";
    std::cout << result_string;
    remove_all(tmp_dir);
    std::cout << "---------------------------------------\n";
    return;
  }
  std::cout << "Sim correct :)\n";
  std::cout << "---------------------------------------\n";
}
