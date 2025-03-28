#pragma once

#include "particle.hpp"
#include "utils/constants.hpp"
#include "reflected/reflection/dual_vector.hpp"

#include <set>
#include <span>
#include <vector>

namespace sim {
struct Block {
  using container_type = acb::dual_vector<Particle>;

  Block() = default;

  void addParticle(auto& particle) {
    particle.acceleration() = gravity;
    particle.density()      = 0;
    particles.push_back(particle);
  }

  void addParticle(Particle& particle) {
    particle.acceleration = gravity;
    particle.density      = 0;
    particles.push_back(particle);
  }

  void calcDensities(FluidProperties const& properties, std::span<u32> adjacent, std::vector<Block>& blocks);

  void calcAccelerations(FluidProperties const& properties, std::span<u32> adjacent, std::vector<Block>& blocks);

  void processCollisions(std::set<Limits>& limits);

  void processLimits(std::set<Limits>& limits);

  void moveParticles();

  container_type particles;
};
} // namespace sim
