
#include "common/args/proargs.hpp"
#include "fld.hpp"
#include "simulator.hpp"

#include <chrono>
#include <iostream>
#include <span>


int main(int const argc, char const* argv[]) {
  std::span const args_view {argv, static_cast<size_t>(argc)};
  auto const init = std::chrono::high_resolution_clock::now();

  auto result = //
      sim::parse_arguments(args_view)
          .and_then(sim::run_simulation)
          .and_then(sim::write_output)
          .transform_error([](std::runtime_error const& error) -> std::string {
            return std::format("Simulation Failed!\n Error: {}", error.what());
          });

  if (not result) {
    std::println("{}", result.error());
    return -1;
  }

  std::chrono::duration<sim::math::scalar> const total = std::chrono::high_resolution_clock::now() - init;
  std::println("Simulation Succeed!");
  std::println("Execution time: {}", total);
  return 0;
}
