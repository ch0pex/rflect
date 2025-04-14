#pragma once

#include "particle.hpp"
#include "utils/constants.hpp"
#include <rflect/rflect.hpp>

#include <set>
#include <span>
#include <vector>

namespace sim {
struct Block {
#ifdef RFLECT_SOA
  using container_type = rflect::dual_vector<Particle, rflect::layout::soa>;
#else
  using container_type = rflect::dual_vector<Particle, rflect::layout::aos>;
#endif

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
