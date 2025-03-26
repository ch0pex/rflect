#pragma once

#include "particle.hpp"
#include "utils/constants.hpp"

#include <set>
#include <span>
#include <vector>

namespace sim {
struct Block {
  Block() = default;

  void addParticle(Particle& particle) {
    particle.acceleration = gravity;
    particle.density      = 0;
    particles.push_back(particle);
  }

  void calcDensities(FluidProperties const& properties, std::span<u32> adjacents, std::vector<Block>& blocks);

  void calcAccelerations(FluidProperties const& properties, std::span<u32> adjacents, std::vector<Block>& blocks);

  void processCollisions(std::set<Limits>& limits);

  void processLimits(std::set<Limits>& limits);

  void moveParticles();

  std::vector<Particle> particles;
};
} // namespace sim
