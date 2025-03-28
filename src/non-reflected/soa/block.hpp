#pragma once

#include "particle.hpp"
#include "utils/constants.hpp"

#include <ranges>
#include <set>
#include <span>
#include <vector>

namespace sim {
class Block {
public:
  Block() = default;

  void addParticle(u32 const id, math::vec3 const& position, math::vec3 const& hv, math::vec3 const velocity) {
    ids.push_back(id);
    positions.push_back(position);
    hvs.push_back(hv);
    velocities.push_back(velocity);
    accelerations.emplace_back(gravity);
    densities.emplace_back(0);
  }

  void calcDensities(FluidProperties const& properties, std::span<u32> adjacent, std::vector<Block>& blocks);

  void calcAccelerations(FluidProperties const& properties, std::span<u32> adjacent, std::vector<Block>& blocks);

  void processCollisions(std::set<Limits>& limits);

  void processLimits(std::set<Limits>& limits);

  void moveParticles();

  auto particles() { return std::views::zip(ids, positions, hvs, velocities, accelerations, densities); }

  [[nodiscard]] auto particles() const { return std::views::zip(ids, positions, hvs, velocities, accelerations, densities); }

  [[nodiscard]] u32 particleCount() const { return ids.size(); }

private:
  std::vector<u32> ids;
  std::vector<math::vec3> positions;
  std::vector<math::vec3> hvs;
  std::vector<math::vec3> velocities;
  std::vector<math::vec3> accelerations;
  std::vector<math::scalar> densities;
};

} // namespace sim
