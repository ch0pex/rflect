#pragma once

#include "block.hpp"
#include "particle.hpp"
#include "math/vector.hpp"
#include "utils/constants.hpp"
#include "utils/primitive_types.hpp"

#include <vector>
#include <map>
#include <set>

namespace sim {
  class Grid {
    public:
      Grid(int np, math::scalar ppm, std::vector<Particle> & particles);

      void repositioning();

      void calculateAccelerations();

      void processCollisions();

      void moveParticles();

      void processLimits();

      [[nodiscard]] int getNumParticles() const { return num_particles; }

      [[nodiscard]] math::scalar getParticlesPerMeter() const { return particles_param_.particles_per_meter; }

      [[nodiscard]] std::vector<Block> & getBlocks() { return blocks_; }

      const ParticlesData& getParameters() { return particles_param_; };

    private:
      void initMessage() const;

      [[nodiscard]] u64 getBlockIndex(math::vec3 particle_pos) const;

      void calculateAdjacentAndLimitBlocks(size_t index);

      [[nodiscard]] bool blockInBounds(math::Vec3<i32> block_pos) const;

      void addBlockToLimits(size_t index, math::Vec3<i32> neighbor_pos);

      int num_particles;
      ParticlesData particles_param_;

      math::Vec3<u64> grid_size_;  // n_x, n_y, n_z
      math::vec3 block_size_;        // s_x, s_y, s_z
      u64 num_blocks_;

      std::vector<Block> blocks_;
      std::vector<std::vector<u64>> adjacent_blocks_;
      std::map<size_t, std::set<Limits>> grid_limits_;
  };
}  // namespace sim
