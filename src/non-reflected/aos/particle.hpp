#ifndef FLUID_PARTICLE_HPP
#define FLUID_PARTICLE_HPP

#include "math/vector.hpp"

namespace sim {

  struct ParticlesData {
    ParticlesData(double ppm);
    double particles_per_meter;
    double smoothing;
    double smoothing_pow_2;
    double smoothing_pow_6;
    double smoothing_pow_9;
    double mass;
    double f45_pi_smooth_6;
    double mass_pressure_05;
    double mass_goo;
  };

  struct Particle {
      Particle(size_t _id, vec3d & _position, vec3d & _hv, vec3d & _velocity);

      Particle(Particle const &);

      void TransformDenisty(const ParticlesData& particles_params);

      static void IncrementDensities(const ParticlesData& particles_params, Particle& particle_i, Particle& particle_j);

      static void IncrementAccelerations(const ParticlesData& particles_params, Particle& particle_i, Particle& particle_j);

      size_t id;

      vec3d position;

      vec3d hv;

      vec3d velocity;

      vec3d acceleration;

      double density;
  };
}  // namespace sim

#endif  // FLUID_PARTICLE_HPP
