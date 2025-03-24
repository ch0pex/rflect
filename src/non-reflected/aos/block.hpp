#include "particle.hpp"
#include "utils/constants.hpp"

#include <vector>
#include <set>

namespace sim {
  class Block {
    public:
      Block() = default;

      void AddParticle(Particle & particle);

      [[nodiscard]] std::vector<Particle> & GetParticles();

      void CalcDensities(ParticlesData const & particles_params, std::vector<size_t>& adjacents, std::vector<Block>& blocks);

      void CalcAccelerations(ParticlesData const & particles_params, std::vector<size_t>& adjacents, std::vector<Block>& blocks);

      void ProcessCollisions(std::set<Limits>& limits);

      void ProcessLimits(std::set<Limits>& limits);

      void MoveParticles();

    private:
      static void CollisionsX(Particle& particle, const std::set<Limits>& limits);

      static void CollisionsY(Particle& particle, const std::set<Limits>& limits);

      static void CollisionsZ(Particle& particle, const std::set<Limits>& limits);

      static void LimitsX(Particle& particle, const std::set<Limits>& limits);

      static void LimitsY(Particle& particle, const std::set<Limits>& limits);

      static void LimitsZ(Particle& particle, const std::set<Limits>& limits);

      std::vector<Particle> particles_; 
  };
}  // namespace sim
