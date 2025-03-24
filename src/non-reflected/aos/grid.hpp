#ifndef ARQUICOMP_P1_GRID_HPP
#define ARQUICOMP_P1_GRID_HPP

#include "block.hpp"
#include "particle.hpp"
#include "math/vector.hpp"

#include <vector>
#include "utils/constants.hpp"
#include <map>
#include <set>

namespace sim {
  class Grid {
    public:
      Grid(int np, double ppm, std::vector<Particle> & particles);

      void Repositioning();
      // Calcula las aceleraciones de las partículas.
      void CalculateAccelerations();
      // Procesa colisiones entre partículas.
      void ProcessCollisions();

      void MoveParticles();

      void ProcessLimits();

      [[nodiscard]] int GetNumParticles() const;

      [[nodiscard]] double GetParticlesPerMeter() const;

      [[nodiscard]] std::vector<Block> & GetBlocks();

      const ParticlesData& GetParameters();

    private:
      void InitMessage() const;

      size_t GetBlockIndex(math::vec3 & particle_pos) const;

      void CalculateAdjacentAndLimitBlocks(size_t index);

      [[nodiscard]] bool BlockInBounds(const math::Vec3<int>& block_pos) const;

      void AddBlockToLimits(size_t index, const math::Vec3<int>& neighbor_pos);

      int num_particles;
      ParticlesData particles_param_;

      math::Vec3<size_t> grid_size_;  // n_x, n_y, n_z
      math::vec3 block_size_;        // s_x, s_y, s_z
      size_t num_blocks_;

      std::vector<Block> blocks_;
      std::vector<std::vector<size_t>> adjacent_blocks_;
      std::map<size_t, std::set<Limits>> grid_limits_;
  };
}  // namespace sim

#endif  // ARQUICOMP_P1_GRID_HPP
