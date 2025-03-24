
#include "simulator.hpp"
#include "common/utils/error.hpp"

#include <chrono>
#include <iostream>
#include <span>



int main(int const argc, char const* argv[]) {
  std::span const args_view {argv, static_cast<size_t>(argc)};
  auto const init          = std::chrono::high_resolution_clock::now();

  sim::Simulator fluid_sim {args_view};
  sim::error_code err      = fluid_sim.ParseArgs();

  if (err == 0) {
    err = fluid_sim.InitSim();
    if (err == 0) {
      fluid_sim.ProcessSim();
      fluid_sim.StoreResults();
    }
  }
  auto const end                            = std::chrono::high_resolution_clock::now();
  std::chrono::duration<sim::math::scalar> const total = end - init;
  std::cout << "Execution time: " << total << "\n";

  return err;
}
