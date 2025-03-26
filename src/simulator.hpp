#pragma once

#include "grid.hpp"
#include "utils/error.hpp"

namespace sim {

struct Arguments {
  i32 iterations;
  std::string input_file;
  std::string output_file;
};

struct Simulation {
  Arguments arguments;
  FluidProperties fluid_properties;
  Grid grid;
};

constexpr auto run_simulation = [](Simulation&& sim) -> err::expected<Simulation> {
    for (int i = 0; i < sim.arguments.iterations; i++) {
      if (i > 0) {
        sim.grid.repositioning();
      }
      sim.grid.calculateAccelerations(sim.fluid_properties);
      sim.grid.processCollisions();
      sim.grid.moveParticles();
      sim.grid.processLimits();
    }
  return std::move(sim);
};

}  // namespace sim
