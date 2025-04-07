#pragma once

#include "block.hpp"
#include "math/vector.hpp"
#include "particle.hpp"

#include <flat_map>
#include <map>
#include <set>
#include <vector>

#include "utils/constants.hpp"

namespace sim {

class Grid {
public:
  explicit Grid(std::ranges::range auto&& particles, math::scalar const smoothing) :
    grid_size_({
      static_cast<u32>(std::floor((top_limit.x - bottom_limit.x) / smoothing)), //
      static_cast<u32>(std::floor((top_limit.y - bottom_limit.y) / smoothing)), //
      static_cast<u32>(std::floor((top_limit.z - bottom_limit.z) / smoothing)), //
    }),
    block_size_({
      (top_limit.x - bottom_limit.x) / static_cast<math::scalar>(grid_size_.x),
      (top_limit.y - bottom_limit.y) / static_cast<math::scalar>(grid_size_.y),
      (top_limit.z - bottom_limit.z) / static_cast<math::scalar>(grid_size_.z),
    }),
    num_blocks_(grid_size_.x * grid_size_.y * grid_size_.z), blocks_(num_blocks_), adjacent_blocks_(num_blocks_) {
    for (auto& particle: particles) {
      u64 const block_index = getBlockIndex(particle.position);
      blocks_[block_index].addParticle(particle); //
    }

    for (u64 i = 0; i < num_blocks_; ++i) {
      calculateAdjacentAndLimitBlocks(i);
    }

    std::cout << "Grid size: " << grid_size_ << "\n";
    std::cout << "Number of blocks: " << num_blocks_ << "\n";
    std::cout << "Block size: " << block_size_ << "\n";
  }

  void repositioning();

  void calculateAccelerations(FluidProperties const& fluid_properties);

  void processCollisions();

  void moveParticles();

  void processLimits();

  [[nodiscard]] std::span<Block const> getBlocks() const { return blocks_; }

private:
  [[nodiscard]] u32 getBlockIndex(math::vec3 const& particle_pos) const;

  void calculateAdjacentAndLimitBlocks(u32 index);

  [[nodiscard]] bool blockInBounds(math::Vec3<i32> const& block_pos) const;

  void addBlockToLimits(u32 index, math::Vec3<i32> const& neighbor_pos);

  math::Vec3<u32> grid_size_; // n_x, n_y, n_z
  math::vec3 block_size_; // s_x, s_y, s_z
  u32 num_blocks_;

  std::vector<Block> blocks_;
  std::vector<std::vector<u32>> adjacent_blocks_;
  std::map<u32, std::set<Limits>> grid_limits_;
};

} // namespace sim
