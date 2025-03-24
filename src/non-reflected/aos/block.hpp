#pragma once

#include "particle.hpp"
#include "utils/constants.hpp"

#include <set>
#include <vector>

namespace sim {

struct Block {
  Block() = default;

  void addParticle(Particle const& particle) { particles.emplace_back(particle); }

  void calcDensities(ParticlesData const& particles_params, std::vector<size_t>& adjacent, std::vector<Block>& blocks);

  void calcAccelerations(ParticlesData const& params, std::vector<size_t>& adjacent, std::vector<Block>& blocks);

  void processCollisions(std::set<Limits>& limits);

  void processLimits(std::set<Limits>& limits);

  void moveParticles();

  std::vector<Particle> particles;
};

} // namespace sim
