#pragma once

#include "block.hpp"
#include "math/vector.hpp"
#include "particle.hpp"

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
      size_t const block_index =
          GetBlockIndex(particle.position); // Coger la posicion de la particula comprobar en que bloque le toca
      blocks_[block_index].AddParticle(particle); // Anadir la particula a dicho bloque
    }

    for (size_t i = 0; i < num_blocks_; ++i) {
      // Se calculan los bloques adjacentes para cada bloque y tambien determina que bloques son limites Cx, Cy, Cz
      CalculateAdjacentAndLimitBlocks(i);
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

  [[nodiscard]] math::scalar GetParticlesPerMeter() const;

  [[nodiscard]] std::vector<Block>& GetBlocks();

  FluidProperties const& GetParameters();

private:
  size_t GetBlockIndex(math::vec3& particle_pos) const;

  void CalculateAdjacentAndLimitBlocks(size_t index);

  [[nodiscard]] bool BlockInBounds(math::Vec3<int> const& block_pos) const;

  void AddBlockToLimits(size_t index, math::Vec3<int> const& neighbor_pos);

  math::Vec3<size_t> grid_size_; // n_x, n_y, n_z
  math::vec3 block_size_; // s_x, s_y, s_z
  size_t num_blocks_;

  std::vector<Block> blocks_;
  std::vector<std::vector<size_t>> adjacent_blocks_;
  std::map<size_t, std::set<Limits>> grid_limits_;
};

} // namespace sim
